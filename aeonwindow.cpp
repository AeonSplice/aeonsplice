#include <string>
#include <gl/glfw.h>
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

bool aeonOpenWindow(std::string title);
bool aeonOpenWindow(std::string title,int width,int height,bool fullscreen);

void aeonCenterMouse();
void aeonCenterCursor();

bool aeonWindowShouldClose();