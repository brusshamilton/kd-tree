
cmake_minimum_required(VERSION 2.8.3)

project(kdtree CXX)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 4)

add_compile_options(--std=c++11 -O3 -Wall)
#add_compile_options(--std=c++11 -O0 -Wall -ggdb -DDEBUG)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(EXTRA_LIBS m)

add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(test)
