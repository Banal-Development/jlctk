cmake_minimum_required(VERSION 3.8)

project(libjlctk-examples CXX)

set (JLCTK_DIR ../libjlctk)

include_directories(${JLCTK_DIR}/src)
#link_directories(${LOCAL_INSTALL_PREFIX}/lib)

add_compile_options(--std=c++23 -g)

add_subdirectory(${JLCTK_DIR}/src libjlctk-build)
add_subdirectory(planets)
