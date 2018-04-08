cmake_minimum_required(VERSION 3.8)

project(Party)

set(PARTY_RUN_TESTS TRUE)

set(CMAKE_CXX_STANDARD 17)

find_package(JNI)

include_directories(${JNI_INCLUDE_DIRS})

set(SOURCE_FILES src/main.cpp src/java_environment.cpp src/java_environment.h)
add_library(Party SHARED ${SOURCE_FILES})

if(PARTY_RUN_TESTS)
    add_subdirectory(test)
endif(PARTY_RUN_TESTS)