#include "pacset_factory.h"

PacsetFactory::PacsetBaseModel* getModel(){
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
}
