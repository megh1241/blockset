#ifndef P_FACTORY
#define P_FACTORY

#include <stdio.h>
#include <iostream>
#include "node.h"
#include "config.h"
#include "pacset_base_model.h"

class PacsetFactory{
    public:
        template<typename T, typename F> PacsetBaseModel<T, F>* getModel();
};
#endif
