#include "aeoncontext.hpp"
// Don't include stuff above here. Because standards.

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

#include "aeonconfig.hpp"
#include "aeonstate.hpp"
#include "aeoninput.hpp"
#include "aeonlog.hpp"
#include "aeonutil.hpp"

using namespace std;
using namespace CEGUI;

namespace aeon
{
    Context::~Context()
    {
        terminateGUI();
        glfwDestroyWindow(aWindowHandle);
    }
    void Context::setContextHint(string target, string hint)
    {
        if(equals(target,"fsaa"))
        {
            try
            {
                int temp = toInt(hint);
                this->setContextHint(GLFW_SAMPLES, temp);
            }
            catch(...)
            {
                this->setContextHint(GLFW_SAMPLES, 0);
            }
        }
        else if(equals(target,"resizable"))
        {
            try
            {
                if(toBoolean(hint))
                {
                    this->setContextHint(GLFW_RESIZABLE, GL_TRUE);
                }
                else
                {
                    this->setContextHint(GLFW_RESIZABLE, GL_FALSE);
                }
            }
            catch(...)
            {
                this->setContextHint(GLFW_RESIZABLE, GL_FALSE);
            }
        }
        else if(equals(target,"decorated"))
        {
            try
            {
                if(toBoolean(hint))
                {
                    this->setContextHint(GLFW_DECORATED, GL_TRUE);
                }
                else
                {
                    this->setContextHint(GLFW_DECORATED, GL_FALSE);
                }
            }
            catch(...)
            {
                this->setContextHint(GLFW_DECORATED, GL_TRUE);
            }
        }
    }
    void Context::setContextHint(int target, int hint)
    {
        glfwWindowHint(target, hint);
    }
    void Context::setContextVersion(int major, int minor)
    {
        this->setContextHint(GLFW_CONTEXT_VERSION_MAJOR, major);
        this->setContextHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    }

    void Context::setWindowPosition(int x, int y)
    {
        glfwSetWindowPos(aWindowHandle, x,y);
    }

    void Context::processInput(int key, int scancode, int action, int mods)
    {
        aState->processInput(key, scancode, action, mods);
    }
    void Context::processChar(unsigned int codepoint)
    {
        aState->processChar(codepoint);
    }
    void Context::processButtons(int button, int action, int mods)
    {
        aState->processButtons(button, action, mods);
    }
    void Context::processFocus(int action)
    {
        aState->processFocus(action);
    }

    static void vodoInput(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Context * temp = static_cast<Context*>(glfwGetWindowUserPointer(window));
        temp->processInput(key, scancode, action, mods);
    }

    static void vodoChar(GLFWwindow* window, unsigned int codepoint)
    {
        Context * temp = static_cast<Context*>(glfwGetWindowUserPointer(window));
        temp->processChar(codepoint);
    }

    static void vodoButtons(GLFWwindow* window, int button, int action, int mods)
    {
        Context * temp = static_cast<Context*>(glfwGetWindowUserPointer(window));
        temp->processButtons(button,action,mods);
    }

    static void vodoFocus(GLFWwindow* window, int action)
    {
        Context * temp = static_cast<Context*>(glfwGetWindowUserPointer(window));
        temp->processFocus(action);
    }

    void Context::openContext()
    {
        aLock.lock();
        GLFWmonitor* prim = glfwGetPrimaryMonitor();
        const GLFWvidmode* desktop = glfwGetVideoMode(prim);
        aWindowHandle = glfwCreateWindow(desktop->width,
                                         desktop->height,
                                         glfwGetMonitorName(prim),
                                         prim,
                                         NULL
                                         );
        if(!aWindowHandle)
        {
            aLock.unlock();
            throw "Failed to create window.";
        }
        else
        {
            aSettings = new Config();
            glfwMakeContextCurrent(aWindowHandle);
            glfwSetWindowUserPointer(aWindowHandle, this);
            glfwSetKeyCallback(aWindowHandle, vodoInput);
            glfwSetCharCallback(aWindowHandle, vodoChar);
            glfwSetMouseButtonCallback(aWindowHandle, vodoButtons);
            glfwSetWindowFocusCallback(aWindowHandle, vodoFocus);
            glfwSetCursorEnterCallback(aWindowHandle, vodoFocus);
            aLock.unlock();
            return;
        }
    }
    void Context::openContext(Config * settings)
    {
        string compilersAreRetarded = "Default Title";
        bool fullscreen =   initKeyPair(settings, "graphics", "fullscreen", false);
        int width =         initKeyPair(settings, "graphics", "width",      800);
        int height =        initKeyPair(settings, "graphics", "height",     600);
        string title =      initKeyPair(settings, "graphics", "title",      compilersAreRetarded);
        aLock.lock();
        aTitle = title;
        if(fullscreen)
        {
            GLFWmonitor* prim = glfwGetPrimaryMonitor();
            const GLFWvidmode* desktop = glfwGetVideoMode(prim);
            aWindowHandle = glfwCreateWindow(desktop->width,
                                             desktop->height,
                                             title.c_str(),
                                             prim,
                                             NULL
                                             );
        }
        else
        {
            aWindowHandle = glfwCreateWindow(width,
                                             height,
                                             title.c_str(),
                                             NULL,
                                             NULL
                                             );
        }
        if(!aWindowHandle)
        {
            aLock.unlock();
            throw "Failed to create window.";
        }
        else
        {
            aSettings = settings;
            glfwMakeContextCurrent(aWindowHandle);
            glfwSetWindowUserPointer(aWindowHandle, this);
            glfwSetKeyCallback(aWindowHandle, vodoInput);
            glfwSetCharCallback(aWindowHandle, vodoChar);
            glfwSetMouseButtonCallback(aWindowHandle, vodoButtons);
            glfwSetWindowFocusCallback(aWindowHandle, vodoFocus);
            glfwSetCursorEnterCallback(aWindowHandle, vodoFocus);
            aLock.unlock();
        }
    }
    void Context::closeContext()
    {
        // NOTE: Does this need locking?
        glfwSetWindowShouldClose(aWindowHandle, GL_TRUE);
    }

    void Context::initGUI()
    {
        OpenGL3Renderer& guiRenderer = OpenGL3Renderer::bootstrapSystem();
        //guiRenderer.enableExtraStateSettings(true);
        // create (load) the TaharezLook scheme file
        // (this auto-loads the TaharezLook looknfeel and imageset files)
        //aeon::log(aeon::getAeonDir());
        // initialise the required dirs for the DefaultResourceProvider
        DefaultResourceProvider* rp = static_cast<DefaultResourceProvider*>
            (System::getSingleton().getResourceProvider());
        //std::string dirToUse = aeon::getAeonDir();
        // NOTE: Hacks...because lazy
        #ifdef _DESKTOP_
        std::string dirToUse = "D:\\Data\\Code\\Repos\\Aeon\\data\\";
        #else
        std::string dirToUse = "C:\\Users\\Josh\\My Code\\Repos\\Aeon\\data\\";
        #endif
        rp->setResourceGroupDirectory("schemes", dirToUse+"schemes/");
        rp->setResourceGroupDirectory("imagesets", dirToUse+"imagesets/");
        rp->setResourceGroupDirectory("fonts", dirToUse+"fonts/");
        rp->setResourceGroupDirectory("layouts", dirToUse+"layouts/");
        rp->setResourceGroupDirectory("looknfeels", dirToUse+"looknfeel/");

        // TODO: Make resource loading use configuration

        // set the default resource groups to be used
        ImageManager::setImagesetDefaultResourceGroup("imagesets");
        Font::setDefaultResourceGroup("fonts");
        Scheme::setDefaultResourceGroup("schemes");
        WidgetLookManager::setDefaultResourceGroup("looknfeels");
        WindowManager::setDefaultResourceGroup("layouts");

        // Load any resources you'll be needing here.
        SchemeManager::getSingleton().createFromFile( "TaharezLook.scheme" );
        SchemeManager::getSingleton().createFromFile( "VanillaSkin.scheme" );
        SchemeManager::getSingleton().createFromFile( "WindowsLook.scheme" );
        FontManager::getSingleton().createFromFile("Jura-10.font");

        //CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-10.font");
        CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("WindowsLook/MouseArrow");
    }

    void Context::terminateGUI()
    {
        return;
    }

    void Context::processExtensions(Config * settings)
    {
        glewExperimental = true; // Needed for core profile
        GLenum GlewInitResult;
        GlewInitResult = glewInit();
        if (GLEW_OK != GlewInitResult)
        {
            throw "Glew failed to initialize.";
        }
    }

    void Context::execute()
    {
        while(!shouldClose())
        {
            aLock.lock();
            aState->executeFrame();
            aLock.unlock();
        }
    }

    void Context::changeState(State * newState)
    {
        aLock.lock();
        // FIXME: Memory leak (old state isn't destroyed)
        aState = newState;
        aLock.unlock();
    }

    void Context::updateFPSCounter()
    {
        static double previous_seconds = glfwGetTime();
        static int frame_count;
        double current_seconds = glfwGetTime();
        double elapsed_seconds = current_seconds - previous_seconds;
        if(elapsed_seconds > 0.25)
        {
            previous_seconds = current_seconds;
            double fps = (double)frame_count / elapsed_seconds;
            std::string temp = aTitle + " - FPS: " + toString(fps);
            glfwSetWindowTitle(aWindowHandle, temp.c_str());
            frame_count = 0;
        }
        frame_count++;
    }

    string Context::getSettingValue(string section, string key)
    {
        return aSettings->getValue(section,key);
    }

    bool Context::shouldClose()
    {
        // NOTE: Does this need locking?
        return glfwWindowShouldClose(aWindowHandle);
    }

    void apiInit()
    {
        if(!glfwInit())
        {
            throw "Failed to initialize GLFW";
        }
    }
    void apiTerminate()
    {
        glfwTerminate();
    }
}
