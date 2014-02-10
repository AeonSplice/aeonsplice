#include "aeonconfig.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "aeonutil.hpp"

using namespace std;

namespace aeon
{
    Config::Config()
    {}
    bool Config::loadFromFile(string file)
    {
        FILE* cfgFile;
        char buffer[100];
        cfgFile = fopen(file.c_str(),"r");
        if(cfgFile==NULL)
            throw "File does not exist.";
        else
        {
            string section;
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
                    string temp = buffer;
                    unsigned pos = temp.find("]");
                    if(pos!=string::npos)
                    {
                        section = temp.substr(1,pos-1);
                        addSection(section);
                    }
                }
                else
                {
                    string temp = buffer;
                    if(temp.at(temp.size()-1)=='\n')
                    {
                        temp.erase(temp.size()-1);
                    }
                    unsigned pos = temp.find("=");
                    if(pos!=string::npos)
                    {
                        string key = temp.substr(0,pos);
                        string value = temp.substr(pos+1);
                        addKeyValue(section,key,value);
                    }
                }
            }
            fclose(cfgFile);
        }
        return true;
    }

    bool Config::saveToFile(string file)
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
                vector<string> fileIOBuffer;
                vector<string> secs = getSections();
                for(vector<string>::iterator sectionIterator = secs.begin();sectionIterator!=secs.end();sectionIterator++)
                {
                    string sectionTemp = "[" + *sectionIterator + "]";
                    fileIOBuffer.push_back(sectionTemp);
                    vector<string> keys = getKeys(*sectionIterator);
                    for(vector<string>::iterator keysIterator = keys.begin();keysIterator!=keys.end();keysIterator++)
                    {
                        string keysTemp = *keysIterator + "=" + getValue(*sectionIterator,*keysIterator);
                        fileIOBuffer.push_back(keysTemp);
                    }
                }
                for(unsigned int bufferIterator = 0; bufferIterator < fileIOBuffer.size(); bufferIterator++)
                {
                    string temp = fileIOBuffer.at(bufferIterator) + "\n";
                    fputs(temp.c_str(),cfgFile);
                }
                fclose(cfgFile);
            }
        }
        else
        {
            vector<string> fileIOBuffer;
            while(!feof(cfgFile))
            {
                if(fgets(buffer,100,cfgFile)==NULL)
                    break;
                string temp = buffer;
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
            string section = "general";

            map< string,bool > completedSections;
            map< string,map<string,bool> > completedKeys;

            vector<string> sections = getSections();
            for(vector<string>::iterator sectionIterator = sections.begin(); sectionIterator!= sections.end(); sectionIterator++)
            {
                completedSections.insert(pair<string,bool>(*sectionIterator,false));
            }
            completedSections.insert(pair<string,bool>("general",false));
            for(vector<string>::iterator sectionIterator = sections.begin(); sectionIterator!= sections.end(); sectionIterator++)
            {
                vector<string> keys = getKeys(*sectionIterator);
                map<string,bool> temp;
                for(vector<string>::iterator keyIterator = keys.begin(); keyIterator!=keys.end(); keyIterator++)
                {
                    temp.insert(pair<string,bool>(*keyIterator,false));
                }
                completedKeys.insert(pair<string,map<string,bool> >(*sectionIterator,temp));
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
                                for(map<string,bool>::iterator keyIterator = completedKeys.at(section).begin(); keyIterator!=completedKeys.at(section).end();keyIterator++)
                                {
                                    if(keyIterator->second == false)
                                    {
                                        string temp = keyIterator->first + "=" + getValue(section,keyIterator->first);
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
                    if(pos!=string::npos)
                    {
                        string currentKey = fileIOBuffer.at(bufferIterator).substr(0,pos);
                        if(exists(section,currentKey))
                        {
                            string temp = currentKey + "=" + getValue(section,currentKey);
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
                        for(map<string,bool>::iterator keyIterator = completedKeys.at(section).begin(); keyIterator!=completedKeys.at(section).end();keyIterator++)
                        {
                            if(keyIterator->second == false)
                            {
                                string temp = keyIterator->first + "=" + getValue(section,keyIterator->first);
                                fileIOBuffer.push_back(temp);
                                completedKeys.at(section).at(keyIterator->first) = true;
                            }
                        }
                    }
                    completedSections.at(section) = true;
                }
            }
            for(map<string,bool>::iterator sectionIterator = completedSections.begin(); sectionIterator!=completedSections.end(); sectionIterator++)
            {
                if(sectionIterator->second == false)
                {
                    if(sectionIterator->first == "general")
                    {
                        if(completedKeys.find(sectionIterator->first)!=completedKeys.end())
                        {
                            for(map<string,bool>::iterator keyIterator = completedKeys.at(sectionIterator->first).begin(); keyIterator!=completedKeys.at(sectionIterator->first).end(); keyIterator++)
                            {
                                if(keyIterator->second==false)
                                {
                                    string temp = keyIterator->first + "=" + getValue(sectionIterator->first,keyIterator->first);
                                    fileIOBuffer.insert(fileIOBuffer.begin(),temp);
                                }
                            }
                        }
                    }
                    else
                    {
                        string temp1 = "[" + sectionIterator->first + "]";
                        fileIOBuffer.push_back(temp1);
                        if(completedKeys.find(sectionIterator->first)!=completedKeys.end())
                        {
                            for(map<string,bool>::iterator keyIterator = completedKeys.at(sectionIterator->first).begin(); keyIterator!=completedKeys.at(sectionIterator->first).end(); keyIterator++)
                            {
                                if(keyIterator->second==false)
                                {
                                    string temp2 = keyIterator->first + "=" + getValue(sectionIterator->first,keyIterator->first);
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
                    string temp = fileIOBuffer.at(bufferIterator) + "\n";
                    fputs(temp.c_str(),cfgFile);
                }
                fclose(cfgFile);
            }
        }
        return true;
    }

    void Config::print()
    {
        for(map< string,map<string,string> >::iterator sec=data.begin();sec!=data.end();sec++)
        {
            cout << "[" << sec->first << "]" << endl;
            for(map<string,string>::iterator key=sec->second.begin();key!=sec->second.end();key++)
            {
                cout << key->first << " => " << key->second << endl;
            }
        }
    }

    vector<string> Config::getSections()
    {
        vector<string> sections;
        for(map< string,map<string,string> >::iterator sec=data.begin();sec!=data.end();sec++)
        {
            sections.push_back(sec->first);
        }
        return sections;
    }
    vector<string> Config::getKeys(string section)
    {
        vector<string> keys;
        if(exists(section))
        {
            for(map<string,string>::iterator key = data.at(section).begin(); key!=data.at(section).end(); key++)
            {
                keys.push_back(key->first);
            }
        }
        return keys;
    }
    string Config::getValue(string section,string key)
    {
        string value;
        if(exists(section,key))
        {
            value = this->data.at(section).at(key);
        }
        return value;
    }

    bool Config::addSection(string section)
    {
        pair< map< string,map<string,string> >::iterator,bool > ret;
        map<string,string> newSection;
        ret = this->data.insert( pair< string,map<string,string> >(section,newSection) );
        if(ret.second==false)
            return false;
        else
            return true;
    }

    bool Config::addKeyValue(string section,string key,string value)
    {
        if(!exists(section))
            addSection(section);
        pair< map<string,string>::iterator,bool > ret;
        ret = this->data.at(section).insert( pair<string,string>(key,value) );
        if(ret.second==false)
            return false;
        else
            return true;

    }

    bool Config::setKeyValue(string section,string key,string value)
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
        pair< map<string,string>::iterator,bool > ret;
        ret = this->data.at(section).insert( pair<string,string>(key,value) );
        if(ret.second==false)
            return false;
        else
            return true;

    }


    bool Config::exists(string section)
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
    bool Config::exists(string section,string key)
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
    int initKeyPair(Config * settings, string section, string key, int defaultValue)
    {
        if(settings->exists(section,key))
        {
            try
            {
                int output = toInt(settings->getValue(section,key));
                return output;
            }
            catch(...)
            {
                settings->setKeyValue(section,key,toString(defaultValue));
                return defaultValue;
            }
        }
        else
        {
            settings->addKeyValue(section,key,toString(defaultValue));
            return defaultValue;
        }
    }
    bool initKeyPair(Config * settings, string section, string key, bool defaultValue)
    {
        if(settings->exists(section,key))
        {
            string value = settings->getValue(section,key);
            try
            {
                bool output = toBoolean(value);
                return output;
            }
            catch(...)
            {
                settings->setKeyValue(section,key,toString(defaultValue));
                return defaultValue;
            }
        }
        else
        {
            settings->addKeyValue(section,key,toString(defaultValue));
            return defaultValue;
        }
    }
    string initKeyPair(Config * settings, string section, string key, string defaultValue)
    {
        if(settings->exists(section,key))
        {
            return settings->getValue(section,key);
        }
        else
        {
            settings->addKeyValue(section,key,defaultValue);
            return defaultValue;
        }
    }
}
