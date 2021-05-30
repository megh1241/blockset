//#include "config.h"
#include "node.h"
#include "stat_node.h"
//#include "pacset_factory.h"
#include "MemoryMapped.h"
//#include "pacset_base_model.h"
#include "pacset_base.h"
#include "utils.h"
#include "packer.h"
#include "json_reader.h"
#include "pacset_rf_classifier.h"
//#include "pacset_rf_regressor.h"
//#include "pacset_gb_regressor.h"
//#include "pacset_gb_classifier.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/iostream.h>
#include <pybind11/numpy.h>

#include <string>

namespace py = pybind11;


/*class PyPacsetBaseModel : public PacsetBaseModel<float, int> {
	using PacsetBaseModel<float, int>::PacsetBaseModel;
	void setMembers(const std::vector<int> &bin_sizes,
                const std::vector<int> &bin_node_sizes,
                const std::vector<std::vector<int>> &bin_start)override { PYBIND11_OVERRIDE_PURE(void, PacsetBaseModel, setMembers, bin_sizes, bin_node_sizes, bin_start); }
}*/	


PYBIND11_MODULE(pyblockset, m) {
m.def("logit2", &logit2);
 m.def("getAccuracy", py::overload_cast<const std::vector<int>&, const std::vector<int>&>(&getAccuracy));
 m.def("loadTestData", py::overload_cast<std::vector<std::vector<float>>&, std::vector<int>&>(&loadTestData) );
 m.def("loadTestData", py::overload_cast<std::vector<std::vector<float>>&, std::vector<double>&>(&loadTestData));
 m.def("getAccuracy", py::overload_cast<const std::vector<int>&, const std::vector<int>&>(&getAccuracy));
 m.def("getAccuracy", py::overload_cast<const std::vector<double>&, const std::vector<double>&>(&getAccuracy));
py::class_<Config>(m, "Config")
    	.def("getInstance", &Config::getInstance)
    	.def("getValue", &Config::getValue)
    	.def("setConfigItem", &Config::setConfigItem);


py::class_<PacsetBase>(m, "PacsetBase")
	.def(py::init<>())
	.def("initRandomForestClassifier", &PacsetBase::initRandomForestClassifier)
	.def("initRandomForestRegressor", &PacsetBase::initRandomForestClassifier)
	.def("initGradientBoostedClassifier", &PacsetBase::initRandomForestClassifier)
	.def("initGradientBoostedRegressor", &PacsetBase::initRandomForestClassifier)
	.def("loadAndPack", &PacsetBase::loadAndPack)
	.def("predict", &PacsetBase::predict);
/* 
   py::class_<PacsetBaseModel<float, int>>(m, "PacsetBaseModel")
        .def("setMembers", &PacsetBaseModel<float, int>::setMembers)
        .def("setBinNodeSizes", &PacsetBaseModel<float, int>::setBinNodeSizes)
        .def("pack", &PacsetBaseModel<float, int>::pack)
        .def("loadModel", &PacsetBaseModel<float, int>::loadModel)
        .def("predict", py::overload_cast<const std::vector<float>&, std::vector<int>&>(&PacsetBaseModel<float, int>::predict))
	.def("predict", py::overload_cast<const std::vector<float>&, std::vector<double>&>(&PacsetBaseModel<float, int>::predict))
        .def("predict", py::overload_cast<const std::vector<std::vector<float>>&, std::vector<int>&, std::vector<int>&, bool>(&PacsetBaseModel<float, int>::predict))
        .def("predict", py::overload_cast<const std::vector<std::vector<float>>&, std::vector<double>&, std::vector<double>&, bool>(&PacsetBaseModel<float, int>::predict))
        .def("serialize", &PacsetBaseModel<float, int>::serialize)
        .def("deserialize", &PacsetBaseModel<float, int>::deserialize);
py::class_< PacsetRandomForestClassifier<float, int>, PacsetBaseModel<float, int>>(m, "PacsetRandomForestClassifier");
    	.def(py::init<>())
	.def("setMembers", &PacsetRandomForestClassifier<float, int>::setMembers)
        .def("setBinNodeSizes", &PacsetRandomForestClassifier<float, int>::setBinNodeSizes)
        .def("pack", &PacsetRandomForestClassifier<float, int>::pack)
        .def("loadModel", &PacsetRandomForestClassifier<float, int>::loadModel)
        .def("predict", py::overload_cast<const std::vector<float>&, std::vector<int>&>(&PacsetRandomForestClassifier<float, int>::predict))
	.def("predict", py::overload_cast<const std::vector<float>&, std::vector<double>&>(&PacsetRandomForestClassifier<float, int>::predict))
        .def("predict", py::overload_cast<const std::vector<std::vector<float>>&, std::vector<int>&, std::vector<int>&, bool>(&PacsetRandomForestClassifier<float, int>::predict))
        .def("predict", py::overload_cast<const std::vector<std::vector<float>>&, std::vector<double>&, std::vector<double>&, bool>(&PacsetRandomForestClassifier<float, int>::predict))
        .def("serialize", &PacsetRandomForestClassifier<float, int>::serialize)
        .def("deserialize", &PacsetRandomForestClassifier<float, int>::deserialize);
    py::class_<PacsetRandomForestRegressor<float, int>, PacsetBaseModel<float, int>>(m, "PacsetRandomForestRegressor")
        .def("setMembers", &PacsetRandomForestRegressor<float, int>::setMembers)
        .def("setBinNodeSizes", &PacsetRandomForestRegressor<float, int>::setBinNodeSizes)
        .def("pack", &PacsetRandomForestRegressor<float, int>::pack)
        .def("loadModel", &PacsetRandomForestRegressor<float, int>::loadModel)
        .def("predict", py::overload_cast<const std::vector<float>&, std::vector<int>&>(&PacsetRandomForestRegressor<float, int>::predict))
	.def("predict", py::overload_cast<const std::vector<float>&, std::vector<double>&>(&PacsetRandomForestRegressor<float, int>::predict))
        .def("predict", py::overload_cast<const std::vector<std::vector<float>>&, std::vector<int>&, std::vector<int>&, bool>(&PacsetRandomForestRegressor<float, int>::predict))
        .def("predict", py::overload_cast<const std::vector<std::vector<float>>&, std::vector<double>&, std::vector<double>&, bool>(&PacsetRandomForestRegressor<float, int>::predict))
        .def("serialize", &PacsetRandomForestRegressor<float, int>::serialize)
        .def("deserialize", &PacsetRandomForestRegressor<float, int>::deserialize);
    
    py::class_<PacsetGradientBoostedClassifier<float, int>, PacsetBaseModel<float, int>>(m, "PacsetGradientBoostedClassifier")
        .def("setMembers", &PacsetGradientBoostedClassifier<float, int>::setMembers)
        .def("setBinNodeSizes", &PacsetGradientBoostedClassifier<float, int>::setBinNodeSizes)
        .def("pack", &PacsetGradientBoostedClassifier<float, int>::pack)
        .def("loadModel", &PacsetGradientBoostedClassifier<float, int>::loadModel)
        .def("predict", py::overload_cast<const std::vector<float>&, std::vector<int>&>(&PacsetGradientBoostedClassifier<float, int>::predict))
	.def("predict", py::overload_cast<const std::vector<float>&, std::vector<double>&>(&PacsetGradientBoostedClassifier<float, int>::predict))
        .def("predict", py::overload_cast<const std::vector<std::vector<float>>&, std::vector<int>&, std::vector<int>&, bool>(&PacsetGradientBoostedClassifier<float, int>::predict))
        .def("predict", py::overload_cast<const std::vector<std::vector<float>>&, std::vector<double>&, std::vector<double>&, bool>(&PacsetGradientBoostedClassifier<float, int>::predict))
        .def("serialize", &PacsetGradientBoostedClassifier<float, int>::serialize)
        .def("deserialize", &PacsetGradientBoostedClassifier<float, int>::deserialize);
    
    py::class_<PacsetGradientBoostedRegressor<float, int>, PacsetBaseModel<float, int>>(m, "PacsetGradientBoostedRegressor")
        .def("setMembers", &PacsetGradientBoostedRegressor<float, int>::setMembers)
        .def("setBinNodeSizes", &PacsetGradientBoostedRegressor<float, int>::setBinNodeSizes)
        .def("pack", &PacsetGradientBoostedRegressor<float, int>::pack)
        .def("loadModel", &PacsetGradientBoostedRegressor<float, int>::loadModel)
        .def("predict", py::overload_cast<const std::vector<float>&, std::vector<int>&>(&PacsetGradientBoostedRegressor<float, int>::predict))
	.def("predict", py::overload_cast<const std::vector<float>&, std::vector<double>&>(&PacsetGradientBoostedRegressor<float, int>::predict))
        .def("predict", py::overload_cast<const std::vector<std::vector<float>>&, std::vector<int>&, std::vector<int>&, bool>(&PacsetGradientBoostedRegressor<float, int>::predict))
        .def("predict", py::overload_cast<const std::vector<std::vector<float>>&, std::vector<double>&, std::vector<double>&, bool>(&PacsetGradientBoostedRegressor<float, int>::predict))
        .def("serialize", &PacsetGradientBoostedRegressor<float, int>::serialize)
        .def("deserialize", &PacsetGradientBoostedRegressor<float, int>::deserialize);
*/
}
