#include "aeonutil.hpp"

#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>

namespace aeon
{
    std::string getUserDir()
    {
        char* path;
        path = getenv("USERPROFILE");
        if(path!=NULL)
        {
            std::stringstream temp;
            std::string output;
            temp << path;
            std::getline(temp,output);
            return output;
        }
        else
        {
            return NULL;
        }
    }
    std::string toString(int number)
    {
       std::stringstream ss;//create a stringstream
       ss << number;//add number to the stream
       return ss.str();//return a string with the contents of the stream
    }
    void createAeonDirectories()
    {
        system("cmd /c mkdir %userprofile%\\.aeonsplice");
    }
}
