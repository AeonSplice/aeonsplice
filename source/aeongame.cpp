#include "aeongame.hpp"
// Don't include stuff above here. Because standards.

#include "aeoncontext.hpp"
#include "aeonconfig.hpp"
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

using namespace std;
using namespace CEGUI;

namespace aeon
{
    // TestInput
    /*void TestInput::setInputSettings(Config * settings)
    {
        setInput("CLOSE", initKeyPair(settings, "input", "CLOSE", GLFW_KEY_ESCAPE));
    }*/

    // LoginState
    LoginState::~LoginState()
    {
        /*glDeleteBuffers(1, &vertexbuffer);
        glDeleteProgram(programID);
        glDeleteVertexArrays(1, &VertexArrayID);*/
    }

    void LoginState::load(Config * settings, GLFWwindow* window, Context * context)
    {
        // TODO: InputHandler
        aSettings = settings;
        aInput = InputHandler();
        aWindowHandle = window;
        contextHandle = context;

        aInput.setWindowHandle(aWindowHandle);
        aInput.setInputSettings(aSettings);

        // Load GUI

        contextHandle->initGUI();

        WindowManager& wmgr = WindowManager::getSingleton();

        Window* myRoot = wmgr.loadLayoutFromFile( "login.layout" );
        System::getSingleton().getDefaultGUIContext().setRootWindow(myRoot);

        registerHandlers();

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }
    void LoginState::registerHandlers()
    {
        try
        {
            CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
            CEGUI::Window * rootWindow = context.getRootWindow();
            rootWindow->getChild("Login/Username")->subscribeEvent(
                                            CEGUI::Editbox::EventTextAccepted,
                                            CEGUI::Event::Subscriber(&LoginState::usernameSubmitted, this));
            rootWindow->getChild("Login/Password")->subscribeEvent(
                                            CEGUI::Editbox::EventTextAccepted,
                                            CEGUI::Event::Subscriber(&LoginState::login, this));
            rootWindow->getChild("Login/Login")->subscribeEvent(
                                            CEGUI::PushButton::EventMouseClick,
                                            CEGUI::Event::Subscriber(&LoginState::login, this));
            rootWindow->getChild("Login/Exit")->subscribeEvent(
                                            CEGUI::PushButton::EventMouseClick,
                                            CEGUI::Event::Subscriber(&LoginState::exit, this));
            rootWindow->getChild("ConnectionFailure/Exit")->subscribeEvent(
                                            CEGUI::PushButton::EventMouseClick,
                                            CEGUI::Event::Subscriber(&LoginState::exit, this));
            rootWindow->getChild("Login/Register")->subscribeEvent(
                                            CEGUI::PushButton::EventMouseClick,
                                            CEGUI::Event::Subscriber(&LoginState::showRegister, this));
            rootWindow->getChild("Login/ForgotPass")->subscribeEvent(
                                            CEGUI::PushButton::EventMouseClick,
                                            CEGUI::Event::Subscriber(&LoginState::showForgotPass, this));
            rootWindow->getChild("Register/Cancel")->subscribeEvent(
                                            CEGUI::PushButton::EventMouseClick,
                                            CEGUI::Event::Subscriber(&LoginState::hideRegister, this));
            rootWindow->getChild("SendPass/Cancel")->subscribeEvent(
                                            CEGUI::PushButton::EventMouseClick,
                                            CEGUI::Event::Subscriber(&LoginState::hideForgotPass, this));
            rootWindow->getChild("LoginFailure/Close")->subscribeEvent(
                                            CEGUI::PushButton::EventMouseClick,
                                            CEGUI::Event::Subscriber(&LoginState::hideLoginFailure, this));
        }
        catch(...)
        {
            log("Failed to register actions.", AEON_ERROR);
        }

    }
    bool LoginState::usernameSubmitted(const CEGUI::EventArgs &e)
    {
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        CEGUI::Window * rootWindow = context.getRootWindow();
        rootWindow->getChild("Login/Password")->activate();
    }
    // Removed for now
    /*bool LoginState::TextSubmitted(const CEGUI::EventArgs &e)
    {
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        CEGUI::Window * rootWindow = context.getRootWindow();
        CEGUI::String msg = rootWindow->getChild("Console/Chatbox")->getText();
        (this)->TextRecieved(msg);
        rootWindow->getChild("Console/Chatbox")->setText("");
        return true;
    }
    void LoginState::TextRecieved(CEGUI::String input)
    {
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        CEGUI::Window * rootWindow = context.getRootWindow();
        CEGUI::Listbox *outputWindow = static_cast<CEGUI::Listbox*>(rootWindow->getChild("Console/History"));
        CEGUI::ListboxItem* newItem = 0;
        newItem = new CEGUI::ListboxTextItem(input);
        outputWindow->addItem(newItem);
    }*/
    bool LoginState::loginToServer(std::string username, std::string password, bool remember)
    {
        // Get Salt
        // Hash Password
        // Send username and hash (and request for session cookie if remember pass enabled)
        // Results
        if(username == "guest" && password == "password")
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool LoginState::login(const CEGUI::EventArgs &e)
    {
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        CEGUI::Window * rootWindow = context.getRootWindow();
        string username = rootWindow->getChild("Login/Username")->getText().c_str();
        string password = rootWindow->getChild("Login/Password")->getText().c_str();
        CEGUI::ToggleButton * rememberpassbox = static_cast<CEGUI::ToggleButton*>(rootWindow->getChild("Login/RememberPass"));
        bool rememberuser = rememberpassbox->isSelected();
        log("User: "+username+"| Pass: "+password+"| Remember: "+toString(rememberuser), AEON_INFO);
        if(loginToServer(username,password,rememberuser))
        {
            log("Logged in.", AEON_INFO);
        }
        else
        {
            log("Failed to log in.", AEON_INFO);
            CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
            CEGUI::Window * rootWindow = context.getRootWindow();
            rootWindow->getChild("Login")->setDisabled(true);
            rootWindow->getChild("LoginFailure")->setVisible(true);
            rootWindow->getChild("Login/Password")->deactivate();
            rootWindow->getChild("Login/Username")->setText("");
            rootWindow->getChild("Login/Password")->setText("");
        }
        return true;
    }
    bool LoginState::exit(const CEGUI::EventArgs &e)
    {
        contextHandle->closeContext();
        return true;
    }
    bool LoginState::hideLoginFailure(const CEGUI::EventArgs &e)
    {
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        CEGUI::Window * rootWindow = context.getRootWindow();
        rootWindow->getChild("Login")->setDisabled(false);
        rootWindow->getChild("LoginFailure")->setVisible(false);
    }
    bool LoginState::showRegister(const CEGUI::EventArgs &e)
    {
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        CEGUI::Window * rootWindow = context.getRootWindow();
        rootWindow->getChild("Login")->setDisabled(true);
        rootWindow->getChild("Register")->setVisible(true);
        rootWindow->getChild("Register/Username")->activate();
        return true;
    }
    bool LoginState::hideRegister(const CEGUI::EventArgs &e)
    {
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        CEGUI::Window * rootWindow = context.getRootWindow();
        rootWindow->getChild("Login")->setDisabled(false);
        rootWindow->getChild("Register")->setVisible(false);
        return true;
    }
    bool LoginState::showForgotPass(const CEGUI::EventArgs &e)
    {
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        CEGUI::Window * rootWindow = context.getRootWindow();
        rootWindow->getChild("Login")->setDisabled(true);
        rootWindow->getChild("SendPass")->setVisible(true);
        rootWindow->getChild("SendPass/Email")->activate();
        return true;
    }
    bool LoginState::hideForgotPass(const CEGUI::EventArgs &e)
    {
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        CEGUI::Window * rootWindow = context.getRootWindow();
        rootWindow->getChild("Login")->setDisabled(false);
        rootWindow->getChild("SendPass")->setVisible(false);
        return true;
    }
    void LoginState::update()
    {
        glfwPollEvents();
        contextHandle->updateFPSCounter();
        double x;
        double y;
        glfwGetCursorPos(contextHandle->aWindowHandle, &x, &y);
        CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
        context.injectMousePosition(x,y);
    }
    void LoginState::render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        CEGUI::System::getSingleton().renderAllGUIContexts();
        glfwSwapBuffers(aWindowHandle);
    }

    // MainContext
    void MainContext::load()
    {
        aLock.lock();
        aState = new LoginState;
        aState->load(aSettings,aWindowHandle,this);
        aLock.unlock();
    }
}
