#include "pacset_factory.h"
#include "pacset_base_model.h"
#include "pacset_rf_classifier.h"
#include "pacset_gb_classifier.h"
#include "pacset_rf_regressor.h"

//TODO: replace GBM and regressors when done

template<typename T, typename F> PacsetBaseModel<T, F> * PacsetFactory::getModel(){
    if (Config::getValue("algorithm") == "randomforest"){
        if( Config::getValue("task") == "classification")
            return new PacsetRandomForestClassifier<float, float>();
        else
            return new PacsetRandomForestRegressor<float, float>();
    }
    else {
        if( Config::getValue("task") == "classification")
            return new PacsetGradientBoostedClassifier<float, float>();
        else
            return new PacsetRandomForestRegressor<float, float>();
    }
}
