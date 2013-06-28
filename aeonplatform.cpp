#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include "aeonplatform.hpp"

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
            temp >> output;
            return output;
        }
        else
        {
            return NULL;
        }
    }
}
