find_path(CATCH_INCLUDE_DIR catch.hpp HINTS "${PROJECT_SOURCE_DIR}/test/third_party" "/usr/include" "/usr/local/include" "/opt/local/include")

if(CATCH_INCLUDE_DIR)
    set(CATCH_FOUND TRUE)

    message(STATUS "Found Catch include at: ${CATCH_INCLUDE_DIR}")
else()
    message(FATAL_ERROR "Failed to locate Catch dependency.")
endif()