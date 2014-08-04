#include "aeonutil.hpp"
// Don't include stuff above here. Because standards.

#include <iostream>
#include <cstdio>
#include <string>
#include <cctype>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include <stdexcept>
#include <windows.h>
#include <time.h>

#include "aeonlog.hpp"

//#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

using namespace std;

namespace aeon
{
    string getUserDir()
    {
        char* path;
        path = getenv("USERPROFILE");
        if(path!=NULL)
        {
            stringstream temp;
            string output;
            temp << path;
            getline(temp,output);
            return output;
        }
        else
        {
            return NULL;
        }
    }
    string getAeonDir()
    {
        char* path;
        path = getenv("APPDATA");
        if(path!=NULL)
        {
            std::stringstream temp;
            std::string output;
            temp << path;
            std::getline(temp,output);
            output+="/.aeonsplice/";
            return output;
        }
        else
        {
            // TODO: Throw error maybe?
            return "./.aeonsplice/";
        }
    }

    bool equals(string first, string second)
    {
        transform(first.begin(), first.end(), first.begin(), ::tolower);
        transform(second.begin(), second.end(), second.begin(), ::tolower);
        if(first == second)
            return true;
        else
            return false;
    }

    const std::string currentDateTime()
    {
        // TODO: datetime function
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

        return buf;
        //return "Temp";
    }

    string toString(int number)
    {
        stringstream ss;
        ss << number;
        return ss.str();
    }
    string toString(unsigned int number)
    {
        stringstream ss;
        ss << number;
        return ss.str();
    }
    string toString(double number)
    {
        double number2 = number + 0.5;
        int temp = (int)number2;
        stringstream ss;
        ss << temp;
        return ss.str();
    }
    string toString(bool boolean)
    {
        if(boolean)
            return "true";
        else
            return "false";
    }
    string toString(const char* str)
    {
        return std::string(str);
    }
    string toString(vector<char> str)
    {
        return std::string(str.data());
    }
    int toInt(string number)
    {
        istringstream ss(number);
        int output;
        if(!(ss >> output))
        {
            throw invalid_argument("Provided non-integer value.");
        }
        else
        {
            return output;
        }
    }
    bool toBoolean(std::string boolean)
    {
        if(equals(boolean,"true") || equals(boolean,"1"))
        {
            return true;
        }
        else if(equals(boolean,"false") || equals(boolean,"0"))
        {
            return false;
        }
        else
        {
            throw invalid_argument("Provided non-boolean value.");
        }
    }
    void initAeonDirectories()
    {
        // NOTE: This code is platform specific, and needs preprocessor tokens when linux compatibility is implemented.
        if(CreateDirectory(getAeonDir().c_str(), NULL))
        {
            log("Succesfully initialized Aeon Splice directories.", AEON_INFO);
        }
        else
        {
            DWORD temp = GetLastError();
            if(temp == ERROR_ALREADY_EXISTS)
            {
                log("Aeon Splice directories already initialized.", AEON_INFO);
            }
            else if(temp == ERROR_ACCESS_DENIED)
            {
                log("Access Denied.", AEON_ERROR);
                throw "403";
            }
            else if(temp == ERROR_PATH_NOT_FOUND)
            {
                log("AppData does not exist?", AEON_ERROR);
                throw "404";
            }
        }
    }

    CEGUI::Key::Scan glfwToCEGUIKey(int glfwKey)
    {
        switch(glfwKey)
        {
            case GLFW_KEY_UNKNOWN	: return CEGUI::Key::Unknown;
            case GLFW_KEY_ESCAPE	: return CEGUI::Key::Escape;
            case GLFW_KEY_A     : return CEGUI::Key::A;
            case GLFW_KEY_B     : return CEGUI::Key::B;
            case GLFW_KEY_C     : return CEGUI::Key::C;
            case GLFW_KEY_D     : return CEGUI::Key::D;
            case GLFW_KEY_E     : return CEGUI::Key::E;
            case GLFW_KEY_F     : return CEGUI::Key::F;
            case GLFW_KEY_G     : return CEGUI::Key::G;
            case GLFW_KEY_H     : return CEGUI::Key::H;
            case GLFW_KEY_I     : return CEGUI::Key::I;
            case GLFW_KEY_J     : return CEGUI::Key::J;
            case GLFW_KEY_K     : return CEGUI::Key::K;
            case GLFW_KEY_L     : return CEGUI::Key::L;
            case GLFW_KEY_M     : return CEGUI::Key::M;
            case GLFW_KEY_N     : return CEGUI::Key::N;
            case GLFW_KEY_O     : return CEGUI::Key::O;
            case GLFW_KEY_P     : return CEGUI::Key::P;
            case GLFW_KEY_Q     : return CEGUI::Key::Q;
            case GLFW_KEY_R     : return CEGUI::Key::R;
            case GLFW_KEY_S     : return CEGUI::Key::S;
            case GLFW_KEY_T     : return CEGUI::Key::T;
            case GLFW_KEY_U     : return CEGUI::Key::U;
            case GLFW_KEY_V     : return CEGUI::Key::V;
            case GLFW_KEY_W     : return CEGUI::Key::W;
            case GLFW_KEY_X     : return CEGUI::Key::X;
            case GLFW_KEY_Y     : return CEGUI::Key::Y;
            case GLFW_KEY_Z     : return CEGUI::Key::Z;
            case GLFW_KEY_F1	: return CEGUI::Key::F1;
            case GLFW_KEY_F2	: return CEGUI::Key::F2;
            case GLFW_KEY_F3	: return CEGUI::Key::F3;
            case GLFW_KEY_F4	: return CEGUI::Key::F4;
            case GLFW_KEY_F5	: return CEGUI::Key::F5;
            case GLFW_KEY_F6	: return CEGUI::Key::F6;
            case GLFW_KEY_F7	: return CEGUI::Key::F7;
            case GLFW_KEY_F8	: return CEGUI::Key::F8;
            case GLFW_KEY_F9	: return CEGUI::Key::F9;
            case GLFW_KEY_F10       : return CEGUI::Key::F10;
            case GLFW_KEY_F11       : return CEGUI::Key::F11;
            case GLFW_KEY_F12       : return CEGUI::Key::F12;
            case GLFW_KEY_F13       : return CEGUI::Key::F13;
            case GLFW_KEY_F14       : return CEGUI::Key::F14;
            case GLFW_KEY_F15       : return CEGUI::Key::F15;
            case GLFW_KEY_UP        : return CEGUI::Key::ArrowUp;
            case GLFW_KEY_DOWN      : return CEGUI::Key::ArrowDown;
            case GLFW_KEY_LEFT      : return CEGUI::Key::ArrowLeft;
            case GLFW_KEY_RIGHT     : return CEGUI::Key::ArrowRight;
            case GLFW_KEY_LEFT_SHIFT    : return CEGUI::Key::LeftShift;
            case GLFW_KEY_RIGHT_SHIFT    : return CEGUI::Key::RightShift;
            case GLFW_KEY_LEFT_CONTROL     : return CEGUI::Key::LeftControl;
            case GLFW_KEY_RIGHT_CONTROL     : return CEGUI::Key::RightControl;
            case GLFW_KEY_LEFT_ALT      : return CEGUI::Key::LeftAlt;
            case GLFW_KEY_RIGHT_ALT      : return CEGUI::Key::RightAlt;
            case GLFW_KEY_TAB       : return CEGUI::Key::Tab;
            case GLFW_KEY_ENTER     : return CEGUI::Key::Return;
            case GLFW_KEY_BACKSPACE : return CEGUI::Key::Backspace;
            case GLFW_KEY_INSERT    : return CEGUI::Key::Insert;
            case GLFW_KEY_DELETE       : return CEGUI::Key::Delete;
            case GLFW_KEY_PAGE_UP    : return CEGUI::Key::PageUp;
            case GLFW_KEY_PAGE_DOWN  : return CEGUI::Key::PageDown;
            case GLFW_KEY_HOME      : return CEGUI::Key::Home;
            case GLFW_KEY_END       : return CEGUI::Key::End;
            case GLFW_KEY_KP_ENTER	: return CEGUI::Key::NumpadEnter;
            default			: return CEGUI::Key::Unknown;
        }
    }

    CEGUI::MouseButton glfwToCEGUIButton(int glfwButton)
    {
        switch(glfwButton)
        {
            case GLFW_MOUSE_BUTTON_LEFT	: return CEGUI::LeftButton;
            case GLFW_MOUSE_BUTTON_RIGHT	: return CEGUI::RightButton;
            case GLFW_MOUSE_BUTTON_MIDDLE	: return CEGUI::MiddleButton;
            default				: return CEGUI::NoButton;
        }
    }
}
