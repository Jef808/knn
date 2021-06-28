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

#include_directories( ${boost_INCLUDE_DIR} )
#include_directories( ${pipes_INCLUDE_DIR} )
include_directories( source_DIR )

#add_executable( iris iris.cpp )
#target_include_directories( iris PUBLIC ${data_DIR} )

add_executable( load_csv_test mnist.cpp )
target_include_directories( load_csv_test PUBLIC ${data_DIR} )

add_executable( console_view console_view.cpp )
target_include_directories( console_view PUBLIC ${data_DIR} )

add_executable( main main.cpp )
target_include_directories( main PUBLIC ${data_DIR} )

add_executable( kaggle_compete kaggle_compete.cpp )
target_include_directories( kaggle_compete PUBLIC ${data_DIR} )

add_executable( stats statistics.cpp )
target_include_directories( stats PUBLIC ${data_DIR} )

find_package(OpenCV REQUIRED)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

# Declare the executable target built from your sources
add_executable(opencv_example opencv_example.cpp)

# Link your application with OpenCV libraries
target_link_libraries(opencv_example PRIVATE ${OpenCV_LIBS})
