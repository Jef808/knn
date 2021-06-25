cmake_minimum_required( VERSION 3.14 )
project( knn LANGUAGES CXX )

set( CMAKE_VERBOSE_MAKEFILE on )
set( CMAKE_EXPORT_COMPILE_COMMANDS on )
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED 20 )
# set( CMAKE_CXX_COMPILER "/usr/bin/g++" )
set( CMAKE_BUILD_TYPE release )

set( source_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
set( data_DIR ${source_DIR}/data )

set( boost_INCLUDE_DIR "/usr/include/boost/" )
set( pipes_INCLUDE_DIR "third_party/pipes/include/" )

include_directories( ${boost_INCLUDE_DIR} )
include_directories( ${pipes_INCLUDE_DIR} )
include_directories( source_DIR )

add_executable( iris iris.cpp )
target_include_directories( iris PUBLIC ${data_DIR} )

add_executable( mnist mnist.cpp )
target_include_directories( mnist PUBLIC ${data_DIR} )

add_executable( main main.cpp )
target_include_directories( main PUBLIC ${data_DIR} )

add_executable( csv mnist_new.cpp )
target_include_directories( csv PUBLIC ${data_DIR} )

add_executable( console_view console_view.cpp )
target_include_directories( console_view PUBLIC ${data_DIR} )
