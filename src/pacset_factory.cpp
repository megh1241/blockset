#include "pacset_factory.h"
#include "pacset_base_model.h"
#include "pacset_rf_classifier.h"
#include "pacset_rf_regressor.h"

template<typename T, typename F> PacsetBaseModel<T, F> * PacsetFactory::getModel(){
    if (Config::getValue("algorithm") == "randomforest"){
        if( Config::getValue("task") == "classification")
            return new PacsetRandomForestClassifier<float, float>();
        else
            return new PacsetRandomForestClassifier<float, float>();
            //return new PacsetRandomForestRegressor<float, float>();
    }
    else {
        if( Config::getValue("task") == "classification")
            return new PacsetRandomForestClassifier<float, float>();
            //return new PacsetGradientBoostedClassifier<float, float>();
        else
            return new PacsetRandomForestClassifier<float, float>();
            //return new PacsetGradientBoostedRegressor<float, float>();
    }
};
