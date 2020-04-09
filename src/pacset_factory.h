#ifndef P_FACTORY
#define P_FACTORY

#include <stdio.h>
#include <iostream>
#include "node.h"
#include "config.h"

class PacsetFactory{
    public:
        PacsetBaseModel* getModel();
};
#endif
