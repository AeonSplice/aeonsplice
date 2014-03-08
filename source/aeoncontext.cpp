#include "aeoncontext.hpp"
// Don't include stuff above here. Because standards.

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "aeonconfig.hpp"
#include "aeonstate.hpp"
#include "aeoninput.hpp"
#include "aeonutil.hpp"

using namespace std;

namespace aeon
{
    Context::~Context()
    {
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
            aLock.unlock();
        }
    }
    void Context::closeContext()
    {
        // NOTE: Does this need locking?
        glfwSetWindowShouldClose(aWindowHandle, GL_TRUE);
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
