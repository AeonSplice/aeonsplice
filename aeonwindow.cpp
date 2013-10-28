#include <string>
#include <sstream>
#include <glfw/glfw3.h>
#include "aeonwindow.hpp"
namespace aeon
{
    GLFWwindow* aeonWindowHandle;
    GLFWwindow* getMainWindowHandle()
    {
        return aeonWindowHandle;
    }
    bool APIInit()
    {
        if(!glfwInit())
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    void APITerminate()
    {
        glfwTerminate();
    }

    void setGLVersion(int major,int minor)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    }

    void setFSAA(int sampling)
    {
        glfwWindowHint(GLFW_SAMPLES, sampling);
    }

    void setResizable(bool canResize)
    {
        if(canResize)
        {
            glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
        }
        else
        {
            glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        }
    }

    /*bool openWindow(std::string title)
    {
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        GLFWvidmode* desktop = glfwGetVideoMode(glfwGetPrimaryMonitor());
        GLFWwindow* mainWindow = glfwCreateWindow(desktop->Width,  // Width of desktop
                                                 desktop->Height, // Height of desktop
                                                 title.c_str(),  // Title of the window
                                                 glfwGetPrimaryMonitor(),           // Monitor to go fullscreen with
                                                 NULL            // Window to share information with (none)
                                                );
        if(!mainWindow)
        {
            return false;
        }
        else
        {
            aeonWindowHandle=mainWindow;
            glfwMakeContextCurrent(aeonWindowHandle);
            return true;
        }
    }*/

    bool openWindow(std::string title,int width,int height,bool fullscreen)
    {
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        GLFWmonitor* prim = glfwGetPrimaryMonitor();
        GLFWwindow* mainWindow;
        if(fullscreen)
        {
        mainWindow = glfwCreateWindow(width,  // Width of desktop
                                      height, // Height of desktop
                                      title.c_str(), // Title of the window
                                      prim,          // Monitor to go fullscreen with
                                      NULL           // Window to share information with (none)
                                     );
        }
        else
        {
        mainWindow = glfwCreateWindow(width,  // Width of desktop
                                      height, // Height of desktop
                                      title.c_str(), // Title of the window
                                      NULL,          // Windowed mode (no monitor specified)
                                      NULL           // Window to share information with (none)
                                     );
        }
        if(!mainWindow)
        {
            return false;
        }
        else
        {
            aeonWindowHandle=mainWindow;
            glfwMakeContextCurrent(aeonWindowHandle);
            return true;
        }
    }

    bool openWindow(config &settings)
    {
        std::string title = settings.getValue("graphics","title");
        std::string fullscreen = settings.getValue("graphics","fullscreen");
        std::string width = settings.getValue("graphics","width");
        std::string height = settings.getValue("graphics","height");
        if(title=="")
        {
            title="Default Title";
            settings.setKeyValue("graphics","title","Default Title");
        }
        int iWidth,iHeight;
        bool fullsrn;
        if(fullscreen == "true" || fullscreen == "1")
        {
            fullsrn=true;
        }
        else if(fullscreen=="false" || fullscreen=="0")
        {
            fullsrn=false;
        }
        else
        {
            fullsrn=false;
            settings.setKeyValue("graphics","fullscreen","false");
        }
        std::istringstream iws(width);
        std::istringstream ihs(height);
        if(!(iws>>iWidth))
        {
            iWidth=800;
            settings.setKeyValue("graphics","width","800");
        }
        if(!(ihs>>iHeight))
        {
            iHeight=600;
            settings.setKeyValue("graphics","height","600");
        }
        return openWindow(title,iWidth,iHeight,fullsrn);
    }

    bool windowShouldClose()
    {
        if(glfwWindowShouldClose(aeonWindowHandle)==GL_TRUE)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    void centerMouse()
    {
        int width,height;
        glfwGetWindowSize(aeonWindowHandle,&width,&height);
        glfwSetCursorPos(aeonWindowHandle,(double)width,(double)height);
    }

    void centerCursor()
    {
        int width,height;
        glfwGetWindowSize(aeonWindowHandle,&width,&height);
        glfwSetCursorPos(aeonWindowHandle,(double)width,(double)height);
    }
}
