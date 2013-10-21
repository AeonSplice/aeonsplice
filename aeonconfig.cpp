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
                    if(temp.at(temp.size()-1)=='\n')
                    {
                        temp.erase(temp.size()-1);
                    }
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

    bool config::saveToFile(std::string file)
    {
        FILE* cfgFile;
        char buffer[100];
        cfgFile = fopen(file.c_str(),"r");
        if(cfgFile==NULL)
        {
            cfgFile = fopen(file.c_str(),"w");
            if(cfgFile==NULL)
                return false;
            else
            {
                std::vector<std::string> fileIOBuffer;
                std::vector<std::string> secs = getSections();
                for(std::vector<std::string>::iterator sectionIterator = secs.begin();sectionIterator!=secs.end();sectionIterator++)
                {
                    std::string sectionTemp = "[" + *sectionIterator + "]";
                    fileIOBuffer.push_back(sectionTemp);
                    std::vector<std::string> keys = getKeys(*sectionIterator);
                    for(std::vector<std::string>::iterator keysIterator = keys.begin();keysIterator!=keys.end();keysIterator++)
                    {
                        std::string keysTemp = *keysIterator + "=" + getValue(*sectionIterator,*keysIterator);
                        fileIOBuffer.push_back(keysTemp);
                    }
                }
                for(unsigned int bufferIterator = 0; bufferIterator < fileIOBuffer.size(); bufferIterator++)
                {
                    std::string temp = fileIOBuffer.at(bufferIterator) + "\n";
                    fputs(temp.c_str(),cfgFile);
                }
                fclose(cfgFile);
            }
        }
        else
        {
            std::vector<std::string> fileIOBuffer;
            while(!feof(cfgFile))
            {
                if(fgets(buffer,100,cfgFile)==NULL)
                    break;
                std::string temp = buffer;
                if(temp.at(temp.size()-1)=='\n')
                {
                    temp.erase(temp.size()-1);
                }
                if(temp.empty())
                {
                    temp = ";";
                }
                fileIOBuffer.push_back(temp);
            }
            fclose(cfgFile);
            // Editing starts here
            std::string section = "general";

            std::map< std::string,bool > completedSections;
            std::map< std::string,std::map<std::string,bool> > completedKeys;

            std::vector<std::string> sections = getSections();
            for(std::vector<std::string>::iterator sectionIterator = sections.begin(); sectionIterator!= sections.end(); sectionIterator++)
            {
                completedSections.insert(std::pair<std::string,bool>(*sectionIterator,false));
            }
            completedSections.insert(std::pair<std::string,bool>("general",false));
            for(std::vector<std::string>::iterator sectionIterator = sections.begin(); sectionIterator!= sections.end(); sectionIterator++)
            {
                std::vector<std::string> keys = getKeys(*sectionIterator);
                std::map<std::string,bool> temp;
                for(std::vector<std::string>::iterator keyIterator = keys.begin(); keyIterator!=keys.end(); keyIterator++)
                {
                    temp.insert(std::pair<std::string,bool>(*keyIterator,false));
                }
                completedKeys.insert(std::pair<std::string,std::map<std::string,bool> >(*sectionIterator,temp));
            }
            for(unsigned int bufferIterator = 0; bufferIterator < fileIOBuffer.size(); bufferIterator++)
            {
                if(fileIOBuffer.at(bufferIterator).at(0)=='[' && fileIOBuffer.at(bufferIterator).at(fileIOBuffer.at(bufferIterator).size()-1)==']')
                {
                    if(completedSections.find(section)!=completedSections.end())
                    {
                        if(completedSections.at(section)==false)
                        {
                            if(completedKeys.find(section)!=completedKeys.end())
                            {
                                for(std::map<std::string,bool>::iterator keyIterator = completedKeys.at(section).begin(); keyIterator!=completedKeys.at(section).end();keyIterator++)
                                {
                                    if(keyIterator->second == false)
                                    {
                                        std::string temp = keyIterator->first + "=" + getValue(section,keyIterator->first);
                                        fileIOBuffer.insert(fileIOBuffer.begin()+(bufferIterator-1), temp);
                                        completedKeys.at(section).at(keyIterator->first) = true;
                                    }
                                }
                            }
                            completedSections.at(section) = true;
                        }
                        section = fileIOBuffer.at(bufferIterator).substr(1,(fileIOBuffer.at(bufferIterator).size()-2));
                    }
                    else
                    {
                        section = fileIOBuffer.at(bufferIterator).substr(1,(fileIOBuffer.at(bufferIterator).size()-2));
                    }
                }
                else if(fileIOBuffer.at(bufferIterator).at(0)==';')
                {}
                else
                {
                    unsigned pos = fileIOBuffer.at(bufferIterator).find("=");
                    if(pos!=std::string::npos)
                    {
                        std::string currentKey = fileIOBuffer.at(bufferIterator).substr(0,pos);
                        if(exists(section,currentKey))
                        {
                            std::string temp = currentKey + "=" + getValue(section,currentKey);
                            fileIOBuffer.at(bufferIterator)=temp;
                        }
                        if(completedKeys.find(section)!=completedKeys.end())
                        {
                            if(completedKeys.at(section).find(currentKey)!=completedKeys.at(section).end())
                            {
                                completedKeys.at(section).at(currentKey) = true;
                            }
                        }
                    }
                }
            }
            if(completedSections.find(section)!=completedSections.end())
            {
                if(completedSections.at(section)==false)
                {
                    if(completedKeys.find(section)!=completedKeys.end())
                    {
                        for(std::map<std::string,bool>::iterator keyIterator = completedKeys.at(section).begin(); keyIterator!=completedKeys.at(section).end();keyIterator++)
                        {
                            if(keyIterator->second == false)
                            {
                                std::string temp = keyIterator->first + "=" + getValue(section,keyIterator->first);
                                fileIOBuffer.push_back(temp);
                                completedKeys.at(section).at(keyIterator->first) = true;
                            }
                        }
                    }
                    completedSections.at(section) = true;
                }
            }
            for(std::map<std::string,bool>::iterator sectionIterator = completedSections.begin(); sectionIterator!=completedSections.end(); sectionIterator++)
            {
                if(sectionIterator->second == false)
                {
                    if(sectionIterator->first == "general")
                    {
                        if(completedKeys.find(sectionIterator->first)!=completedKeys.end())
                        {
                            for(std::map<std::string,bool>::iterator keyIterator = completedKeys.at(sectionIterator->first).begin(); keyIterator!=completedKeys.at(sectionIterator->first).end(); keyIterator++)
                            {
                                if(keyIterator->second==false)
                                {
                                    std::string temp = keyIterator->first + "=" + getValue(sectionIterator->first,keyIterator->first);
                                    fileIOBuffer.insert(fileIOBuffer.begin(),temp);
                                }
                            }
                        }
                    }
                    else
                    {
                        std::string temp1 = "[" + sectionIterator->first + "]";
                        fileIOBuffer.push_back(temp1);
                        if(completedKeys.find(sectionIterator->first)!=completedKeys.end())
                        {
                            for(std::map<std::string,bool>::iterator keyIterator = completedKeys.at(sectionIterator->first).begin(); keyIterator!=completedKeys.at(sectionIterator->first).end(); keyIterator++)
                            {
                                if(keyIterator->second==false)
                                {
                                    std::string temp2 = keyIterator->first + "=" + getValue(sectionIterator->first,keyIterator->first);
                                    fileIOBuffer.push_back(temp2);
                                }
                            }
                        }
                    }
                }
            }
            // Editing ends here
            cfgFile = fopen(file.c_str(),"w");
            if(cfgFile==NULL)
                return false;
            else
            {
                for(unsigned int bufferIterator = 0; bufferIterator < fileIOBuffer.size(); bufferIterator++)
                {
                    std::string temp = fileIOBuffer.at(bufferIterator) + "\n";
                    fputs(temp.c_str(),cfgFile);
                }
                fclose(cfgFile);
            }
        }
        return true;
    }

    void config::setReadOnly(bool readonly)
    {
        isreadonly = readonly;
    }
    void config::setIgnoreWarnings(bool ignorewarnings)
    {
        isignorewarnings = ignorewarnings;
    }

    void config::print()
    {
        for(std::map< std::string,std::map<std::string,std::string> >::iterator sec=data.begin();sec!=data.end();sec++)
        {
            std::cout << "[" << sec->first << "]" << std::endl;
            for(std::map<std::string,std::string>::iterator key=sec->second.begin();key!=sec->second.end();key++)
            {
                std::cout << key->first << " => " << key->second << std::endl;
            }
        }
    }

    std::vector<std::string> config::getSections()
    {
        std::vector<std::string> sections;
        for(std::map< std::string,std::map<std::string,std::string> >::iterator sec=data.begin();sec!=data.end();sec++)
        {
            sections.push_back(sec->first);
        }
        return sections;
    }
    std::vector<std::string> config::getKeys(std::string section)
    {
        std::vector<std::string> keys;
        if(exists(section))
        {
            for(std::map<std::string,std::string>::iterator key = data.at(section).begin(); key!=data.at(section).end(); key++)
            {
                keys.push_back(key->first);
            }
        }
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

    bool config::setKeyValue(std::string section,std::string key,std::string value)
    {
        if(!exists(section))
            addSection(section);
        else
        {
            if(exists(section,key))
            {
                this->data.at(section).erase(key);
            }
        }
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
