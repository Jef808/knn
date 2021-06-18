cmake_minimum_required( VERSION 3.14 )
project( knn LANGUAGES CXX )

set( CMAKE_VERBOSE_MAKEFILE on )
set( CMAKE_EXPORT_COMPILE_COMMANDS on )
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_COMPILER "/usr/bin/g++" )
set( CMAKE_BUILD_TYPE debug )

set( source_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
set( data_DIR ${source_DIR}/data )

add_executable( iris iris.cpp )
target_include_directories( iris PUBLIC source_DIR data_DIR )

add_executable( mnist mnist.cpp )
target_include_directories( mnist PUBLIC source_DIR data_DIR )
