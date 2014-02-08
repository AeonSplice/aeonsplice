﻿#include "aeonutil.hpp"

#include <iostream>
#include <cstdio>
#include <string>
#include <cctype>
#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include <windows.h>

#include "aeonlog.hpp"

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

    string toString(int number)
    {
       stringstream ss;
       ss << number;
       return ss.str();
    }
    string toString(bool boolean)
    {
        if(boolean)
            return "true";
        else
            return "false";
    }
    int toInt(string number)
    {
        istringstream ss(number);
        int output;
        if(!(ss >> output))
        {
            throw invalid_argument("Provided none-integer value.");
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
            throw invalid_argument("Provided none-boolean value.");
        }
    }
    void initAeonDirectories()
    {
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
                log("Access Denied", AEON_ERROR);
                throw "403";
            }
            else if(temp == ERROR_PATH_NOT_FOUND)
            {
                log("AppData does not exist?", AEON_ERROR);
                throw "404";
            }
        }
    }
}
