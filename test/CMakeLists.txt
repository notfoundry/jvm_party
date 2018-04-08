set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/test/cmake/modules ${CMAKE_MODULE_PATH})

find_package(Catch)

if(CATCH_FOUND)
    add_library(Catch INTERFACE)

    target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

        include_directories(${CMAKE_SOURCE_DIR}/src)

    set(TEST_SOURCES ${CMAKE_SOURCE_DIR}/test/src/tests_main.cpp src/tests_example.cpp)
    add_executable(tests ${TEST_SOURCES})
    target_link_libraries(tests Catch "-static")

    enable_testing()

    add_test(NAME test COMMAND $<TARGET_FILE_NAME:tests>)
endif(CATCH_FOUND)