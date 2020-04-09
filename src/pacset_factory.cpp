#ifndef P_FACTORY
#define P_FACTORY

#include <stdio.h>
#include <iostream>
#include "node.h"
#include "config.h"

class PacsetFactory{
    public:
        PacsetBaseModel* getModel(){
            if (Config::getValue("algorithm") == "randomforest"){
                if( Config::getValue("task") == "classification")
                    return new PacsetRandomForestClassifier();
                else
                    return new PacsetRandomForestRegressor();
            }
            else {
                if( Config::getValue("task") == "classification")
                    return new PacsetGradientBoostedClassifier();
                else
                    return new PacsetGradientBoostedRegressor();
            }
        }
        
};
#endif
