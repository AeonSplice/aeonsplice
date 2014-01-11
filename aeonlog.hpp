#ifndef _AEONLOG_
#define _AEONLOG_
#include "aeonconfig.hpp"
namespace aeon
{
    const std::string currentDateTime();
    void log(std::string message);
    void getLogSettings(config * settings);
    void setLogFile(std::string file);
    FILE* getLogFile();
}
#endif
