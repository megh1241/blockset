#ifndef PACSET_RF_CLASS
#define PACSET_RF_CLASS

#include <vector>
#include "pacset_base_model.h"

template <typename T, typename F>
class PacsetRandomForestClassifier: public PacsetBaseModel<T, F> {
    public:
        void pack();
        int predict(const std::vector<T, F> observation);
        int predict(const std::vector<std::vector<T, F>> observation);
        void serialize();
        void deserialize();
};

#endif
