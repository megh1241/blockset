cmake_minimum_required(VERSION 3.8)
project (exe)


set(CMAKE_CXX_STANDARD 14)
#set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
#find_package(PythonLibs ${PY_VERSION} REQUIRED)
#add_subdirectory(pybind11)

#pybind11_add_module(exe src/bindings.cpp)

add_executable(exe src/MemoryMapped.h src/MemoryMapped.cpp src/config.h  src/config.cpp src/config.h src/utils.h src/utils.cpp src/pacset_base.h src/pacset_base.cpp  src/pacset_base_model.h src/pacset_rf_classifier.h src/pacset_rf_regressor.h src/pacset_factory.h src/pacset_factory.cpp src/json_reader.cpp src/json_reader.h src/stat_node.cpp src/stat_node.h src/node.h  src/main.cpp)
target_include_directories(exe PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 -march=native")

#set(JSON_BuildTests OFF CACHE INTERNAL "")

#find_package(nlohmann_json 3.2.0 REQUIRED)

#include_directories(${PYTHON_INCLUDE_DIRS})
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(exe PUBLIC OpenMP::OpenMP_CXX)
endif()

