#ifndef _AEONINPUT_
#define _AEONINPUT_

#include <string>

namespace aeon
{
    class Config;

    void getInputSettings(Config * settings);
    void setInput(std::string key,int value);
    bool inputExists(std::string input);
    int getInput(std::string input);

    bool isMouseDown(std::string button);
    bool isKeyDown(std::string key);
}

#endif // _AEONINPUT_
