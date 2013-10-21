#include <string>
#include <sstream>
#include <GL/glfw.h>
#include "aeonconfig.hpp"
#include "aeonwindow.hpp"
namespace aeon
{
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
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, major);
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, minor);
    }

    void setFSAA(int sampling)
    {
        glfwOpenWindowHint(GLFW_FSAA_SAMPLES, sampling);
    }
    void setResizable(bool canResize)
    {
        if(canResize)
        {
            glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_FALSE);
        }
        else
        {
            glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
        }
    }

    bool openWindow(std::string title)
    {
        GLFWvidmode desktop;
        glfwGetDesktopMode(&desktop);
        if(!glfwOpenWindow(desktop.Width,desktop.Height,desktop.RedBits,desktop.GreenBits,desktop.BlueBits,8,32,0,GLFW_FULLSCREEN))
        {
            return false;
        }
        else
        {
            glfwSetWindowTitle(title.c_str());
            return true;
        }
    }
    bool openWindow(std::string title,int width,int height,bool fullscreen)
    {
        // Setting the RGB bit depths to 0 causes it to automatically use the desktop bit depths.
        if(fullscreen)
        {
            if(!glfwOpenWindow(width,height,0,0,0,8,32,0,GLFW_FULLSCREEN))
            {
                return false;
            }
            else
            {
                glfwSetWindowTitle(title.c_str());
                return true;
            }
        }
        else
        {
            if(!glfwOpenWindow(width,height,0,0,0,8,32,0,GLFW_WINDOW))
            {
                return false;
            }
            else
            {
                glfwSetWindowTitle(title.c_str());
                return true;
            }
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
            title="Wut";
            settings.setKeyValue("graphics","title","Wut");
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
        /*std::cout << title << std::endl;
        std::cout << iWidth << std::endl;
        std::cout << iHeight << std::endl;
        std::cout << fullsrn << std::endl;*/
        return openWindow(title,iWidth,iHeight,fullsrn);
    }

    void closeWindow()
    {
        glfwCloseWindow();
    }

    void setWindowTitle(std::string title)
    {
        glfwSetWindowTitle(title.c_str());
    }

    void centerMouse()
    {
        int width,height;
        glfwGetWindowSize(&width,&height);
        glfwSetMousePos(width/2,height/2);
    }
    void centerCursor()
    {
        int width,height;
        glfwGetWindowSize(&width,&height);
        glfwSetMousePos(width/2,height/2);
    }

    bool windowShouldClose()
    {
        if(glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS && glfwGetWindowParam( GLFW_OPENED ))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}
