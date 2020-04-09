#ifndef PACSET_RF_CLASS
#define PACSET_RF_CLASS

#include <vector>
#include "pacset_base_model.h"
#include "packer.h"
#include "config.h"

template <typename T, typename F>
class PacsetRandomForestClassifier: public PacsetBaseModel<T, F> {
    public:
        inline void pack(){
            std::string layout = Config::getValue("layout");
            Packer<T, F> packer_obj(layout);
            packer_obj.pack(PacsetBaseModel<T, F>::bins);
        }

        inline int predict(const std::vector<T, F> observation) {
        }
        inline int predict(const std::vector<std::vector<T, F>> observation) {
        }
        inline void serialize() {
        }
        inline void deserialize() {
        }
};

#endif
