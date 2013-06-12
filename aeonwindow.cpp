#include <string>
#include <GL/glfw.h>
#include "aeonwindow.hpp"

bool aeonAPIInit()
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
void aeonAPITerminate()
{
    glfwTerminate();
}

void aeonSetGLVersion(int major,int minor)
{
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, major);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, minor);
}

void aeonSetFSAA(int sampling)
{
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, sampling);
}
void aeonSetResizable(bool canResize)
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

bool aeonOpenWindow(std::string title)
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
bool aeonOpenWindow(std::string title,int width,int height,bool fullscreen)
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

void aeonCenterMouse()
{
    int width,height;
    glfwGetWindowSize(&width,&height);
    glfwSetMousePos(width/2,height/2);
}
void aeonCenterCursor()
{
    int width,height;
    glfwGetWindowSize(&width,&height);
    glfwSetMousePos(width/2,height/2);
}

bool aeonWindowShouldClose()
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
