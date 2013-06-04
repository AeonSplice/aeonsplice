#include <GL/glfw.h>
#include "aeonwindow.hpp"

int aeonWidth,aeonHeight;

int windowWidth()
{
	return aeonWidth;
}

int windowHeight()
{
	return aeonHeight;
}

void aeonSetFSAA(int aavalue)
{
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, aavalue);
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

void getWindowResolution(int x*,int y*)
{
	glfwGetWindowResolution(&x,&y); // IS THIS HERP DERPED? Double check later
}

void setGLVersion(int major, int minor)
{
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, major);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, minor);
}

bool openWindow()
{
	GLFWvidmode desktop;
	glfwGetDesktopMode(&desktop);
	aeonWidth=desktop.Width;
	aeonHeight=desktop.Height;
	return glfwOpenWindow(desktop.Width, desktop.Height, desktop.RedBits, desktop.GreenBits,desktop.BlueBits, 8, 32, 8, GLFW_FULLSCREEN);
}

bool openWindow(int width, int height, bool fullscreen)
{
	aeonWidth=width;
	aeonHeight=height;
	GLFWvidmode desktop;
	glfwGetDesktopMode(&desktop);
	if(fullscreen)
	{
		return glfwOpenWindow(width, height, desktop.RedBits, desktop.GreenBits,desktop.BlueBits, 8, 32, 8, GLFW_FULLSCREEN);
	}
	else
	{
		return glfwOpenWindow(width, height, desktop.RedBits, desktop.GreenBits,desktop.BlueBits, 8, 32, 8, GLFW_WINDOW);
	}
}

bool openWindow(int width, int height, int redBits, int greenBits, int blueBits, int alphaBits, int depthBits, int stencilBits, bool fullscreen)
{
	aeonWidth=width;
	aeonHeight=height;
	if(fullscreen)
	{
		return glfwOpenWindow(width, height, redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, GLFW_FULLSCREEN);
	}
	else
	{
		return glfwOpenWindow(width, height, redBits, greenBits, blueBits, alphaBits, depthBits, stencilBits, GLFW_WINDOW);
	}
}

void centerMouse()
{
	glfwSetMousePos(aeonWidth/2,aeonHeight/2);
}