#ifndef CONFIG_H
#define CONFIG_H

#include <fstream>
#include <map>
#include <string>

//Singleton class to store global configuration information

class Config 
{
    public:
        static Config& getInstance();
        static std::string getValue(std::string key);
        static void setConfigItem(std::string item, std::string value);
        //TODO: set defaults
    private:
        Config(){};
        Config(const Config&);
        Config& operator=(const Config&);
        static std::map<std::string, std::string> config_map;
};

#endif
