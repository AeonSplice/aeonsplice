#include "aeonserver.hpp"
// Don't include stuff above here. Because standards.

#include "aeoncontext.hpp"
#include "aeonconfig.hpp"
#include "aeonshader.hpp"
#include "aeoninput.hpp"
#include "aeonlog.hpp"
#include "aeonutil.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

#include <string>
#include <thread>

#include <tinyxml2.h>

using namespace std;
using namespace CEGUI;

namespace aeon
{
    void ServerInput::setInputSettings(Config * settings)
    {
        setInput("CLOSE", initKeyPair(settings, "input", "CLOSE", GLFW_KEY_ESCAPE));
    }

    ServerState::~ServerState()
    {
        // TODO: Things
        aServerHandle->destroy();
    }

    void ServerState::load(Config * settings, GLFWwindow* window, Context * context)
    {
        aSettings = settings;
        aInput = InputHandler();
        aWindowHandle = window;
        contextHandle = context;

        aInput.setWindowHandle(aWindowHandle);
        aInput.setInputSettings(aSettings);

        contextHandle->initGUI();

        WindowManager& wmgr = WindowManager::getSingleton();

        Window* myRoot = wmgr.loadLayoutFromFile( "server.layout" );
        System::getSingleton().getDefaultGUIContext().setRootWindow(myRoot);

        registerHandlers();

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Now we start network because reasons.
        aServerHandle = new ServerConnection();
        aServerHandle->create(DEFAULT_PORT);
    }

    void ServerState::registerHandlers()
    {
        try
        {
            CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
            CEGUI::Window * rootWindow = context.getRootWindow();
            rootWindow->getChild("Console/Submit")->subscribeEvent(
                                            CEGUI::PushButton::EventMouseClick,
                                            CEGUI::Event::Subscriber(&ServerState::TextSubmitted, this));
            rootWindow->getChild("Console/Chatbox")->subscribeEvent(
                                            CEGUI::Editbox::EventTextAccepted,
                                            CEGUI::Event::Subscriber(&ServerState::TextSubmitted, this));
        }
        catch(...)
        {
            log("Failed to register actions.", AEON_ERROR);
        }

    }

    void ServerState::processInput(int key, int scancode, int action, int mods)
    {
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            contextHandle->closeContext();
        else
        {
            if(action == GLFW_PRESS)
            {
                context.injectKeyDown(glfwToCEGUIKey(key));
            }
            else if(action == GLFW_RELEASE)
            {
                context.injectKeyUp(glfwToCEGUIKey(key));
            }
            else if(action == GLFW_REPEAT)
            {
                // TODO: Banana phone
                context.injectKeyDown(glfwToCEGUIKey(key));
            }
        }
    }
    void ServerState::processChar(unsigned int codepoint)
    {
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        context.injectChar(codepoint);
    }
    void ServerState::processButtons(int button, int action, int mods)
    {
        try
        {
            CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
            if(action == GLFW_PRESS)
            {
                context.injectMouseButtonDown(glfwToCEGUIButton(button));
            }
            else if(action == GLFW_RELEASE)
            {
                context.injectMouseButtonUp(glfwToCEGUIButton(button));
            }
        }
        catch(...)
        {
            aeon::log("This is not the button state you're looking for.", AEON_ERROR);
        }
    }

    void ServerState::processFocus(int action)
    {
        try
        {
            CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
            if(action == GL_TRUE)
            {
                context.getMouseCursor().show();
                //log("Focused");
            }
            else if(action == GL_FALSE)
            {
                context.getMouseCursor().hide();
                //log("Not Focused");
            }
            else
                log("glfw focus callback returned none boolean value.", AEON_ERROR);
        }
        catch(...)
        {
            aeon::log("Failed to focus?", AEON_ERROR);
        }
    }

    bool ServerState::TextSubmitted(const CEGUI::EventArgs &e)
    {
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        CEGUI::Window * rootWindow = context.getRootWindow();
        CEGUI::String msg = rootWindow->getChild("Console/Chatbox")->getText();
        // TODO: Abstract this shit.
        if(msg == "/exit")
        {
            (this)->TextRecieved("Exiting...");
            rootWindow->getChild("Console/Chatbox")->setText("");
            contextHandle->closeContext();
            return true;
        }
        else if(msg == "/?" || msg == "/help")
        {
            (this)->TextRecieved("/? or /help \t- Display help.");
            (this)->TextRecieved("/exit \t- Exit program.");
            (this)->TextRecieved("/clear \t- Clear chat history.");
            rootWindow->getChild("Console/Chatbox")->setText("");
            return true;
        }
        else if(msg == "/clear")
        {
            rootWindow->getChild("Console/Chatbox")->setText("");
            CEGUI::Listbox *outputWindow = static_cast<CEGUI::Listbox*>(rootWindow->getChild("Console/History"));
            outputWindow->resetList();
            return true;
        }
        /*else if(msg == "/time")
        {
            ip::udp::resolver resolver(my_io_service);
            ip::udp::resolver::query query("localhost", "daytime");
            ip::udp::resolver::iterator iter = resolver.resolve(query);
            iter
        }*/
        (this)->TextRecieved(msg);
        rootWindow->getChild("Console/Chatbox")->setText("");
        return true;
    }
    void ServerState::TextRecieved(CEGUI::String input)
    {
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        CEGUI::Window * rootWindow = context.getRootWindow();
        CEGUI::Listbox *outputWindow = static_cast<CEGUI::Listbox*>(rootWindow->getChild("Console/History"));
        CEGUI::ListboxItem* newItem = 0;
        newItem = new CEGUI::ListboxTextItem(input);
        outputWindow->addItem(newItem);
    }

    void ServerState::update()
    {
        glfwPollEvents();
        contextHandle->updateFPSCounter();
        aServerHandle->process();
        double x;
        double y;
        glfwGetCursorPos(contextHandle->aWindowHandle, &x, &y);
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        context.injectMousePosition(x,y);
    }

    void ServerState::render()
    {
        // Clear buffer.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Write GUI to buffer.
        CEGUI::System::getSingleton().renderAllGUIContexts();
        // Swap current buffer with buffer we just rendered to.
        glfwSwapBuffers(aWindowHandle);
    }



    void ServerContext::load()
    {
        aLock.lock();
        aState = new ServerState;
        aState->load(aSettings,aWindowHandle,this);
        aLock.unlock();
    }
}
