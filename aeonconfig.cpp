#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "aeonconfig.hpp"

namespace aeon
{
    config::config()
    {
        std::cout << "INFO: F*** YOU I'M AN OCTOPUS" << std::endl;
    }
    bool config::loadFromFile(std::string file)
    {
        FILE* cfgFile;
        char buffer[100];
        cfgFile = fopen(file.c_str(),"r");
        if(cfgFile==NULL)
            return false;
        else
        {
            std::string section;
            section = "general";
            addSection(section);
            while(!feof(cfgFile))
            {
                if(fgets(buffer,100,cfgFile)==NULL)
                    break;
                if(buffer[0]==';'||buffer[0]=='\n')
                {
                    // do nothing
                }
                else if(buffer[0]=='[')
                {
                    std::string temp = buffer;
                    unsigned pos = temp.find("]");
                    if(pos!=std::string::npos)
                    {
                        section = temp.substr(1,pos-1);
                        addSection(section);
                    }
                }
                else
                {
                    std::string temp = buffer;
                    unsigned pos = temp.find("=");
                    if(pos!=std::string::npos)
                    {
                        std::string key = temp.substr(0,pos);
                        std::string value = temp.substr(pos+1);
                        addKeyValue(section,key,value);
                    }
                    // process temp string (which copied the data from the buffer)
                }
            }
            fclose(cfgFile);
        }
        return true;
    }
    bool config::loadFromString(std::string)
    {
        // Place holders are fun :D
        return false;
    }

    void config::setReadOnly(bool readonly)
    {

    }
    void config::setIgnoreWarnings(bool ignorewarnings)
    {

    }

    void config::print()
    {
        for(std::map< std::string,std::map<std::string,std::string> >::iterator sec=data.begin();sec!=data.end();sec++)
        {
            std::cout << "[" << sec->first << "]" << std::endl;
            for(std::map<std::string,std::string>::iterator key=sec->second.begin();key!=sec->second.end();key++)
            {
                std::cout << key->first << "=>" << key->second << std::endl;
            }
        }
    }

    std::vector<std::string> config::getSections()
    {
        std::vector<std::string> sections;
        return sections;
    }
    std::vector<std::string> config::getKeys(std::string section)
    {
        std::vector<std::string> keys;
        return keys;
    }
    std::string config::getValue(std::string section,std::string key)
    {
        std::string value;
        if(exists(section,key))
        {
            value = this->data.at(section).at(key);
        }
        return value;
    }

    bool config::addSection(std::string section)
    {
        std::pair< std::map< std::string,std::map<std::string,std::string> >::iterator,bool > ret;
        std::map<std::string,std::string> newSection;
        ret = this->data.insert( std::pair< std::string,std::map<std::string,std::string> >(section,newSection) );
        if(ret.second==false)
            return false;
        else
            return true;
    }

    bool config::addKeyValue(std::string section,std::string key,std::string value)
    {
        if(!exists(section))
            addSection(section);
        std::pair< std::map<std::string,std::string>::iterator,bool > ret;
        ret = this->data.at(section).insert( std::pair<std::string,std::string>(key,value) );
        if(ret.second==false)
            return false;
        else
            return true;

    }


    bool config::exists(std::string section)
    {
        if(this->data.find(section)!=this->data.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool config::exists(std::string section,std::string key)
    {
        if(this->data.find(section)!=this->data.end())
        {
            if(this->data.at(section).find(key)!=this->data.at(section).end())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    std::string getDefaultConfigString()
    {
        std::string result;

        return result;
    }
}
