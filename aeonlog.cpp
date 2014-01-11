#include <string>
#include <fstream>
#include <iostream>
#include <time.h>
#include "aeonconfig.hpp"
#include "aeonlog.hpp"

namespace aeon
{
    bool isDebug = true;
    bool overwriteLog = false;
    std::string logLocation = "";
    const std::string currentDateTime()
    {
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
        // for more information about date/time format
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

        return buf;
    }
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
    void getLogSettings(config * settings)
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
                    std::cout << "ERROR: Failed to write to log file." << std::endl;
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
}
