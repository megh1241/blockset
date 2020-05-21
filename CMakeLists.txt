cmake_minimum_required(VERSION 3.8)
project (exe)
#set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
add_executable(exe src/MemoryMapped.h src/MemoryMapped.cpp src/config.h  src/config.cpp src/config.h src/utils.h src/utils.cpp src/pacset_base_model.h src/pacset_rf_classifier.h src/pacset_rf_regressor.h src/pacset_factory.h src/pacset_factory.cpp src/json_reader.cpp src/json_reader.h src/stat_node.cpp src/stat_node.h src/node.h src/main.cpp)
find_package(PythonLibs ${PY_VERSION} REQUIRED)

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(${nlohmann_json_source}
   ${CMAKE_CURRENT_BINARY_DIR}/json)


#find_package(nlohmann_json 3.2.0 REQUIRED)



include_directories(${PYTHON_INCLUDE_DIRS})
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(exe PUBLIC OpenMP::OpenMP_CXX)
endif()

target_link_libraries(exe PRIVATE ${PYTHON_LIBRARIES}  nlohmann_json::nlohmann_json)
