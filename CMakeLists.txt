cmake_minimum_required(VERSION 3.15...4.0)
project(example LANGUAGES CXX)

find_package(pybind11 REQUIRED)
find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(mapnik CONFIG REQUIRED)
pybind11_add_module(mapnikpywin MODULE main.cpp)
target_link_libraries(mapnikpywin PRIVATE mapnik::mapnik mapnik::json mapnik::wkt)

# mapnik_find_plugin_dir(MAPNIK_PLUGIN_DIR)

pybind11_extension(mapnikpywin)
if(NOT MSVC AND NOT ${CMAKE_BUILD_TYPE} MATCHES Debug|RelWithDebInfo)
    # Strip unnecessary sections of the binary on Linux/macOS
    pybind11_strip(mapnikpywin)
endif()

set_target_properties(mapnikpywin PROPERTIES CXX_VISIBILITY_PRESET "hidden"
                                         CUDA_VISIBILITY_PRESET "hidden")