#include <string>
#include <GL/glfw.h>
#include "aeonwindow.hpp"
using namespace std;

bool aeonAPIInit()
{

}

void aeonAPITerminate()
{

}

void aeonSetGLVersion(int major,int minor)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
}

void aeonSetFSAA(int sampling)
{
    glfwWindowHint(GLFW_SAMPLES, sampling);
}

void aeonSetReizable(bool canResize)
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

bool aeonOpenWindow(string title)
{
    aeonSetWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWmonitor prim = glfwGetPrimaryMonitor();
    GLFWvidmode desktop = glfwGetVideoMode(prim);
    GLFWwindow mainWindow = glfwCreateWindow(desktop.Width,  // Width of desktop
                                             desktop.Height, // Height of desktop
                                             title.c_str(),  // Title of the window
                                             prim,           // Monitor to go fullscreen with
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
}

bool aeonOpenWindow(string title,int width,int height,bool fullscreen)
{
    aeonSetWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWmonitor prim = glfwGetPrimaryMonitor();
    GLFWwindow mainWindow;
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