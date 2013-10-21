#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <Windows.h>
#include "aeonplatform.hpp"

namespace aeon
{
    std::string getUserDir()
    {
        char* path;
        path = getenv("USERPROFILE");
        if(path!=NULL)
        {
            // std::cout << path << std::endl; // DEBUGGING CODE REMOVE LATER
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
    void createDir(std::string folder)
    {
        CreateDirectory(folder,NULL);
    }
}
