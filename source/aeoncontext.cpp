#include "aeoncontext.hpp"
// Don't include stuff above here. Because standards.

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "aeonconfig.hpp"
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
                    this->setContextHint(GLFW_RESIZABLE, GL_TRUE);
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
                    this->setContextHint(GLFW_DECORATED, GL_TRUE);
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
        // TODO: aLock.lock();
        const GLFWvidmode* desktop = glfwGetVideoMode(glfwGetPrimaryMonitor());
        GLFWwindow* temp = glfwCreateWindow(desktop->width,
                                            desktop->height,
                                            "Untitled",
                                            glfwGetPrimaryMonitor(),
                                            NULL
                                            );
        if(!temp)
        {
            // TODO: aLock.unlock();
            throw "Failed to create window.";
        }
        else
        {
            aWindowHandle=temp;
            glfwMakeContextCurrent(aWindowHandle);
            // TODO: aLock.unlock();
            return;
        }
    }
    void Context::openContext(Config * settings)
    {
        bool fullscreen = initKeyPair(settings, "graphics", "fullscreen", false);
        int width = initKeyPair(settings, "graphics", "width", 800);
        int height = initKeyPair(settings, "graphics", "height", 600);
        string compilersAreRetarded = "Default Title";
        string title = initKeyPair(settings, "graphics", "title", compilersAreRetarded);
        if(fullscreen)
        {
            GLFWmonitor* prim = glfwGetPrimaryMonitor();
            aWindowHandle = glfwCreateWindow(width,
                                             height,
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
        glfwMakeContextCurrent(aWindowHandle);
        if(!aWindowHandle)
        {
            throw "Failed to create window.";
        }
    }
    void Context::closeContext()
    {
        glfwSetWindowShouldClose(aWindowHandle, GL_TRUE);
    }

    void Context::processExtensions(Config * settings)
    {
        /*glewExperimental = true; // Needed for core profile
        GLenum GlewInitResult;
        GlewInitResult = glewInit();
        if (GLEW_OK != GlewInitResult)
        {
            throw "Glew failed to initialize.";
        }*/
    }

    bool Context::shouldClose()
    {
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
