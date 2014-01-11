#include <string>
#include <map>
#include <glfw/glfw3.h>
#include "aeonwindow.hpp"
#include "aeonconfig.hpp"
#include "aeoninput.hpp"

using namespace std;

namespace aeon
{
    map<string,int> knownInputs;

    void getInputSettings(config * settings)
    {
        setInput("LMB",initKeyPair(settings, "input", "LMB", GLFW_MOUSE_BUTTON_LEFT));
        setInput("RMB",initKeyPair(settings, "input", "RMB", GLFW_MOUSE_BUTTON_RIGHT));
        setInput("CAMERA_FORWARD",initKeyPair(settings, "input", "CAMERA_FORWARD", GLFW_KEY_W));
        setInput("CAMERA_BACK",initKeyPair(settings, "input", "CAMERA_BACK", GLFW_KEY_S));
        setInput("CAMERA_LEFT",initKeyPair(settings, "input", "CAMERA_LEFT", GLFW_KEY_A));
        setInput("CAMERA_RIGHT",initKeyPair(settings, "input", "CAMERA_RIGHT", GLFW_KEY_D));
        setInput("CAMERA_UP",initKeyPair(settings, "input", "CAMERA_UP", GLFW_KEY_Q));
        setInput("CAMERA_DOWN",initKeyPair(settings, "input", "CAMERA_DOWN", GLFW_KEY_Z));
    }
    void setInput(string key,int value)
    {
        if(inputExists(key))
        {
            knownInputs.erase(key);
        }
        knownInputs.insert( pair<string,int>(key,value));
    }

    bool inputExists(string input)
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
    int getInput(string input)
    {
        if(inputExists(input))
        {
            return knownInputs.at(input);
        }
        return 0;
    }

    bool isMouseDown(string button)
    {
        if(inputExists(button))
        {
            return glfwGetMouseButton(getMainWindowHandle(), getInput(button));
        }
        else
        {
            return false;
        }
    }

    bool isKeyDown(string key)
    {
        if(inputExists(key))
        {
            return glfwGetKey(getMainWindowHandle(), getInput(key));
        }
        else
        {
            return false;
        }
    }
}
