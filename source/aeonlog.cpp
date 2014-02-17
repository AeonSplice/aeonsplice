#include "aeonlog.hpp"
// Don't include stuff above here. Because standards.

#include <iostream>
//#include <ctime>

#include "aeonconfig.hpp"
#include "aeonutil.hpp"

using namespace std;

namespace aeon
{
    bool isDebug = true;
    bool overwriteLog = false;
    std::string logLocation = "";
    void overwriteLogFile()
    {
        if(overwriteLog)
        {
            FILE* logFile;
            logFile = fopen(logLocation.c_str(),"w");
            if(logFile==NULL)
            {
                std::cout << "ERROR: Failed to overwrite log file." << std::endl;
            }
            else
            {
                fclose(logFile);
            }
        }
    }
    void getLogSettings(Config * settings)
    {
        isDebug = initKeyPair(settings, "debug", "isDebugMode", true);
        overwriteLog = initKeyPair(settings, "debug", "overwriteLog", false);
    }
    void setLogFile(std::string file)
    {
        logLocation = file;
    }
    FILE* getLogFile()
    {
        FILE* logFile;
        logFile = fopen(logLocation.c_str(),"a");
        return logFile;
    }
    void log(std::string message)
    {
        if(isDebug)
        {
            std::cout << message << std::endl;
            if(!logLocation.empty())
            {
                FILE* logFile;
                logFile = fopen(logLocation.c_str(),"a");
                if(logFile==NULL)
                {
                    std::cout << "ERROR - Failed to write to log file." << std::endl;
                }
                else
                {
                    std::string temp;
                    if(message.at(message.size()-1)!='\n')
                    {
                        temp = currentDateTime()+" - "+message+"\n";
                    }
                    else
                    {
                        temp = currentDateTime()+" - "+message;
                    }
                    fputs(temp.c_str(),logFile);
                    fclose(logFile);
                }
            }
        }
        else
        {
            std::cout << message << std::endl;
        }
    }
    void log(const char* message)
    {
        log(toString(message));
    }
    void log(string message, int mode)
    {
        // TODO: Utilize Configuration for verboseness.
        switch(mode)
        {
        case AEON_FATAL:
            log("FATAL - "+message);
            break;
        case AEON_ERROR:
            log("ERROR - "+message);
            break;
        case AEON_WARNING:
            log("WARNING - "+message);
            break;
        case AEON_INFO:
            log("INFO - "+message);
            break;
        default:
            log("Tried to use invalid log mode \""+toString(mode)+"\"", AEON_WARNING);
            log("UNKNOWN - "+message);
            break;
        }
    }
    void log(const char* message, int mode)
    {
        log(toString(message), mode);
    }
}
