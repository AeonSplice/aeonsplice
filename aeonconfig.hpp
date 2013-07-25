﻿#ifndef _AEONCONFIG_
#define _AEONCONFIG_
#include <vector>
#include <map>
namespace aeon
{
    class config
    {
    public:
        config();
        /**
            Loads values from file/string, overwrites if values are prexisting. (overwriting not implemented yet...)
        */
        bool loadFromFile(std::string file);
        bool loadFromString(std::string);
        /**
            Loads values from file/string, does not overwrite. (ensures that certain values always exist)
        */
        bool loadDefaultsFromFile(std::string file);
        bool loadDefaultsFromString(std::string);
        /**
            Saves current configuration to file. (useful if you change something during runtime, and want to apply it.)
        */
        bool saveToFile(std::string file);

        void setReadOnly(bool readyonly);
        void setIgnoreWarnings(bool ignorewarnings);

        void print();

        std::vector<std::string> getSections();
        std::vector<std::string> getKeys(std::string section);
        std::string getValue(std::string section,std::string key);
        bool addSection(std::string section);
        bool addKeyValue(std::string section,std::string key,std::string value);
        bool setKeyValue(std::string section,std::string key,std::string value);
        bool exists(std::string section);
        bool exists(std::string section,std::string key);
    private:
        bool readonly;
        bool ignorewarnings;
        std::map< std::string, std::map<std::string,std::string> > data;
    };
    std::string getDefaultConfigString();
}

#endif
