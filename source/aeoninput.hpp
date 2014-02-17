#ifndef _AEONINPUT_
#define _AEONINPUT_

#include <GLFW/glfw3.h>

#include <string>

namespace aeon
{
    class Config;

    class InputHandler
    {
    public:
        InputHandler();
        ~InputHandler();

        void setWindowHandle(GLFWwindow * windowHandle);
        virtual void setInputSettings(Config * settings);

        void setInput(std::string key,int value);
        int getInput(std::string input);
        bool inputExists(std::string input);

        bool isMouseDown(std::string button);
        bool isKeyDown(std::string key);
    private:
        map<string,int> knownInputs;
        GLFWwindow * aWindowHandle;
    };
}

#endif // _AEONINPUT_
