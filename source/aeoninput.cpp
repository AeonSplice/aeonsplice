#include "aeoninput.hpp"
// Don't include stuff above here. Because standards.

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <map>

#include "aeonconfig.hpp"

using namespace std;

namespace aeon
{
    InputHandler::InputHandler()
    {
        // TODO: InputHandler Constructor
    }
    InputHandler::~InputHandler()
    {
        // TODO: InputHandler Destructor
    }
    void InputHandler::setWindowHandle(GLFWwindow * windowHandle)
    {
        aWindowHandle = windowHandle;
    }
    void InputHandler::setInputSettings(Config * settings)
    {
        setInput("LMB",             initKeyPair(settings, "input", "LMB",               GLFW_MOUSE_BUTTON_LEFT));
        setInput("RMB",             initKeyPair(settings, "input", "RMB",               GLFW_MOUSE_BUTTON_RIGHT));
        setInput("CAMERA_FORWARD",  initKeyPair(settings, "input", "CAMERA_FORWARD",    GLFW_KEY_W));
        setInput("CAMERA_BACK",     initKeyPair(settings, "input", "CAMERA_BACK",       GLFW_KEY_S));
        setInput("CAMERA_LEFT",     initKeyPair(settings, "input", "CAMERA_LEFT",       GLFW_KEY_A));
        setInput("CAMERA_RIGHT",    initKeyPair(settings, "input", "CAMERA_RIGHT",      GLFW_KEY_D));
        setInput("CAMERA_UP",       initKeyPair(settings, "input", "CAMERA_UP",         GLFW_KEY_Q));
        setInput("CAMERA_DOWN",     initKeyPair(settings, "input", "CAMERA_DOWN",       GLFW_KEY_Z));
        setInput("CLOSE",           initKeyPair(settings, "input", "CLOSE",             GLFW_KEY_ESCAPE));
        setInput("DERP",            initKeyPair(settings, "input", "DERP",              GLFW_KEY_F12));
    }
    void InputHandler::setInput(string key,int value)
    {
        if(inputExists(key))
        {
            knownInputs.erase(key);
        }
        knownInputs.insert( pair<string,int>(key,value));
    }

    bool InputHandler::inputExists(string input)
    {
        if(knownInputs.find(input)!=knownInputs.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    int InputHandler::getInput(string input)
    {
        if(inputExists(input))
        {
            return knownInputs.at(input);
        }
        else
        {
            throw invalid_argument("Input not found.");
        }
    }

    bool InputHandler::isMouseDown(string button)
    {
        if(inputExists(button))
        {
            return glfwGetMouseButton(aWindowHandle, getInput(button));
        }
        else
        {
            throw invalid_argument("Input not found.");
        }
    }

    bool InputHandler::isKeyDown(string key)
    {
        if(inputExists(key))
        {
            return glfwGetKey(aWindowHandle, getInput(key));
        }
        else
        {
            throw invalid_argument("Input not found.");
        }
    }

    void InputHandler::processKeyInput()
    {
        // TODO: This is the callback for glfw key events.
    }
}
