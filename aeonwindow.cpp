#include <string>
#include <sstream>
#include <glfw/glfw3.h>
#include "aeonconfig.hpp"
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

    void setDecorated(bool isDecorated)
    {
        if(isDecorated)
        {
            glfwWindowHint(GLFW_DECORATED, GL_TRUE);
        }
        else
        {
            glfwWindowHint(GLFW_DECORATED, GL_FALSE);
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

    bool openWindow(std::string title,int width,int height,bool fullscreen,bool useCoreProfile)
    {
        if(useCoreProfile)
        {
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }
        else
        {
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
        }
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

    bool openWindow(config * settings)
    {
        std::string wtfIsWrongWithMyCompiler = "Default Title";
        std::string title = initKeyPair(settings, "graphics", "title", wtfIsWrongWithMyCompiler);
        bool fullscreen = initKeyPair(settings, "graphics", "fullscreen", false);
        int width = initKeyPair(settings, "graphics", "width", 800);
        int height = initKeyPair(settings, "graphics", "height", 600);
        wtfIsWrongWithMyCompiler = "coreProfile";
        std::string profile = initKeyPair(settings, "debug", "profile", wtfIsWrongWithMyCompiler);
        bool coreProf;
        if(profile == "coreProfile")
        {
            coreProf = true;
        }
        else if(profile == "anyProfile")
        {
            coreProf = false;
        }
        else
        {
            coreProf = true;
            settings->setKeyValue("debug","profile","coreProfile");
        }
        return openWindow(title,width,height,fullscreen,coreProf);
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

    void getMousePos(int * x, int * y)
    {
        double width,height;
        glfwGetCursorPos(aeonWindowHandle,&width,&height);
        *x = (int)width;
        *y = (int)height;
    }
    void getMousePos(double * x, double * y)
    {
        glfwGetCursorPos(aeonWindowHandle,x,y);
    }

    void setMousePos(int x,int y)
    {
        glfwSetCursorPos(aeonWindowHandle,(double)x,(double)y);
    }
    void setMousePos(double x,double y)
    {
        glfwSetCursorPos(aeonWindowHandle,x,y);
    }

    void centerMouse()
    {
        int width,height;
        glfwGetWindowSize(aeonWindowHandle,&width,&height);
        glfwSetCursorPos(aeonWindowHandle,(double)(width/2),(double)(height/2));
    }

    void hideCursor(bool hide)
    {
        if(hide)
        {
            glfwSetInputMode(aeonWindowHandle,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);
        }
        else
        {
            glfwSetInputMode(aeonWindowHandle,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
        }
    }

    float getWindowWidth()
    {
        int width,height;
        glfwGetWindowSize(aeonWindowHandle,&width,&height);
        return width;
    }
    float getWindowHeight()
    {
        int width,height;
        glfwGetWindowSize(aeonWindowHandle,&width,&height);
        return height;
    }
    float getWindowRatio()
    {
        int width,height;
        float output;
        glfwGetWindowSize(aeonWindowHandle,&width,&height);
        output = width/height;
        return output;
    }
}
