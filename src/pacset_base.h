#ifndef BASE
#define BASE

#include <vector>
#include <iostream>
#include "pacset_base_model.h"
#include "pacset_factory.h"
#include "utils.h"
#include "config.h"
#include "pacset_rf_classifier.h"
#include "pacset_rf_regressor.h"
#include "pacset_gb_classifier.h"
#include "pacset_gb_regressor.h"

class PacsetBase{
    //TODO: (Note) Upcaset to Node when serializing to file
        PacsetBaseModel<float, float> *obj;
    public:
	PacsetBase(){}
	void initRandomForestClassifier();
	void initRandomForestRegressor();
	void initGradientBoostedClassifier();
	void initGradientBoostedRegressor();
	void loadAndPack();
	void predict();
};

#endif
