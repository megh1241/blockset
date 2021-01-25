#include "config.h"
#include <map>

std::map<std::string, std::string> Config::config_map;

Config& Config::getInstance(){
    static Config instance;
    return instance;
}

std::string Config::getValue(std::string key){
    if(config_map.find(key) != config_map.end())
        return config_map[key];
    return std::string("notfound");
}

void Config::setConfigItem(std::string item, std::string value){
    Config::config_map[item] = value; 
}
