cmake_minimum_required(VERSION 3.16)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" STRING "Minimum OS X deployment version")
set(CMAKE_CXX_STANDARD 17)

project(Prefabs)

set(PLUGIN_NAME ${PROJECT_NAME})
set(ADDITIONAL_PLUGIN_DISTRIBUTABLES README.md)

include_directories(src)
include_directories(efsw/include)
include_directories(efsw/src)

include(RackSDK.cmake)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE EFSW_SOURCES "efsw/src/efsw/*.cpp")

target_sources(${RACK_PLUGIN_LIB} PRIVATE ${SOURCE_FILES} ${EFSW_SOURCES})

message(STATUS "CMAKE_OSX_DEPLOYMENT_TARGET: ${CMAKE_OSX_DEPLOYMENT_TARGET}")




