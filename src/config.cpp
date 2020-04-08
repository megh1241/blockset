#include "config.h"
#include <map>

std::map<std::string, std::string> Config::config_map;

Config& Config::getInstance(){
    static Config instance;
    return instance;
}

std::string Config::getValue(std::string key){
    return config_map[key];
}

void Config::setConfigItem(std::string item, std::string value){
    Config::config_map[item] = value; 
}
