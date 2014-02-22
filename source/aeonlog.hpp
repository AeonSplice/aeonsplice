#ifndef _AEONLOG_
#define _AEONLOG_

#include <string>
#include <fstream>

namespace aeon
{
    class Config;

    void log(std::string message);
    void log(const char* message);
    void log(std::string message, int mode);
    void log(const char* message, int mode);
    void getLogSettings(Config * settings);
    void setLogFile(std::string file);
    FILE* getLogFile();
}
#endif // _AEONLOG_
