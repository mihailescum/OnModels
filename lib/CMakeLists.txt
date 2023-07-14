project(lib LANGUAGES CXX)

add_library(onmodels INTERFACE)
target_include_directories(onmodels INTERFACE .)
target_compile_features(onmodels INTERFACE cxx_std_17)