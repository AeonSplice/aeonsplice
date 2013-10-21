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
        std::cout << file << std::endl; // DEBUGGING CODE REMOVE LATER
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

    bool config::saveToFile(std::string file)
    {
        std::cout << file << std::endl; // DEBUGGING CODE REMOVE LATER
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
            std::map<std::string, bool> completedSections;
            std::map<std::string, std::map<std::string, bool> > completedValues;


            std::vector<std::string> secs = getSections();

            for(std::vector<std::string>::iterator it = secs.begin(); it!=secs.end();it++)
            {
                completedSections.insert(std::pair<std::string,bool>(*it,false));
            }
            for(std::vector<std::string>::iterator it2 = secs.begin(); it2!=secs.end();it2++)
            {
                std::vector<std::string> keysTmp = getKeys(*it2);
                std::map<std::string,bool> temp;
                for(std::vector<std::string>::iterator it3 = keysTmp.begin(); it3!=keysTmp.end();it3++)
                {
                    temp.insert(std::pair<std::string,bool>(*it3,false));
                }
                completedValues.insert(std::pair<std::string,std::map<std::string,bool> >(*it2,temp));
            }
            std::vector<std::string> fileIOBuffer;
            while(!feof(cfgFile))
            {
                if(fgets(buffer,100,cfgFile)==NULL)
                    break;
                std::string temp = buffer;
                fileIOBuffer.push_back(temp);
            }
            fclose(cfgFile);
            unsigned int sweeper = 0;
            std::string section;
            section = "general";
            while(sweeper<fileIOBuffer.size())
            {
                if(fileIOBuffer.at(sweeper).at(0)=='[' && fileIOBuffer.at(sweeper).at(fileIOBuffer.at(sweeper).size()-1)==']')
                {
                    if(completedSections.at(section)==true)
                    {
                        section = fileIOBuffer.at(sweeper).substr(1,(fileIOBuffer.at(sweeper).size()-2));
                        sweeper++;
                    }
                    else
                    {
                        for(std::map<std::string,bool>::iterator iter1 = completedValues.at(section).begin(); iter1 != completedValues.at(section).end(); iter1++)
                        {
                            if(iter1->second==false)
                            {
                                std::cout << iter1->first << std::endl;
                                std::string asdf = iter1->first + "=" + getValue(section,iter1->first);
                                fileIOBuffer.insert(fileIOBuffer.begin()+(sweeper-1), asdf);
                                iter1->second=true;
                            }
                        }
                        completedSections.at(section)=true;
                        section = fileIOBuffer.at(sweeper).substr(1,(fileIOBuffer.at(sweeper).size()-2));
                        sweeper++;
                    }
                }
                else if(fileIOBuffer.at(sweeper).at(0)==';')
                {
                    sweeper++;
                }
                else
                {
                    unsigned pos = fileIOBuffer.at(sweeper).find("=");
                    if(pos!=std::string::npos)
                    {
                        std::string currentKey = fileIOBuffer.at(sweeper).substr(0,pos);
                        if(exists(section,currentKey))
                        {
                            std::string temp = currentKey + "=" + getValue(section,currentKey);
                            fileIOBuffer.at(sweeper)=temp;
                            sweeper++;
                        }
                        else
                        {
                            sweeper++;
                        }
                    }
                    else
                    {
                        sweeper++;
                    }
                }
            }
            for(std::map<std::string,bool>::iterator secIt = completedSections.begin();secIt!=completedSections.end();secIt++)
            {
                // This is to print out all the sections that didn't exist to the end of the file
                if(secIt->second==false)
                {
                    std::string temp1 = "[" + secIt->first + "]";
                    fileIOBuffer.push_back(temp1);
                    for(std::map<std::string,bool>::iterator keyIt=completedValues.at(secIt->first).begin(); keyIt!=completedValues.at(secIt->first).end();keyIt++)
                    {
                        if(keyIt->second==false)
                        {
                            if(exists(secIt->first,keyIt->first))
                            {
                                std::string temp2 = keyIt->first + "=" + getValue(secIt->first,keyIt->first);
                                fileIOBuffer.push_back(temp2);
                            }
                        }
                    }
                }
            }
            cfgFile = fopen(file.c_str(),"w");
            if(cfgFile==NULL)
                return false;
            else
            {
                for(unsigned int iter4 = 0; iter4 < fileIOBuffer.size(); iter4++)
                {
                    //std::cout << fileIOBuffer.at(iter4) << std::endl;
                    std::string temp = fileIOBuffer.at(iter4) + "\n";
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
