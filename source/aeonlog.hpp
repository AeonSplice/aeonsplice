#ifndef _AEONLOG_
#define _AEONLOG_
namespace aeon
{
    void log(std::string message);
    void getLogSettings(Config * settings);
    void setLogFile(std::string file);
    FILE* getLogFile();
}
#endif // _AEONLOG_