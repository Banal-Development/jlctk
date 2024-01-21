cmake_minimum_required(VERSION 3.8)

project(libjlctk CXX)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src ${LOCAL_INSTALL_PREFIX}/include)
#link_directories(${LOCAL_INSTALL_PREFIX}/lib)

add_compile_options(--std=c++23 -g)

add_subdirectory(src)
