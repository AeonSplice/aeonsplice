#include "aeoncontext.hpp"
// Don't include stuff above here. Because standards.

#include <string>

#include <GL/glfw3.h>

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

    bool Context::openContext()
    {
        // TODO: lock
        const GLFWvidmode* desktop = glfwGetVideoMode(glfwGetPrimaryMonitor());
        GLFWwindow* temp = glfwCreateWindow(desktop->width,
                                            desktop->height,
                                            "Untitled",
                                            glfwGetPrimaryMonitor(),
                                            NULL
                                            );
        if(!temp)
        {
            // TODO: unlock
            return false;
        }
        else
        {
            aWindowHandle=temp;
            glfwMakeContextCurrent(aWindowHandle);
            // TODO: unlock
            return true;
        }
    }
    bool Context::openContext(Config * settings)
    {
        return false;
    }
    void Context::closeContext()
    {
        glfwSetWindowShouldClose(aWindowHandle, GL_TRUE);
    }

    void Context::processExtensions(Config * settings)
    {
        // TODO: Initialize and process Glew.
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
