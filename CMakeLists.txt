cmake_minimum_required(VERSION 2.8)
project(mindbw)

set(DB_VERSION "0.0.1")

################################
# Third party libraries
################################
find_package(SQLite3 REQUIRED)

find_package(kizhi)
find_package(zephyr)

include_directories(${KIZHI_INCLUDE_DIRS})
include_directories(${SQLITE3_INCLUDE_DIR})
include_directories(${ZEPHYR_INCLUDE_DIRS})

################################
# Locate source
################################
file(GLOB_RECURSE INCLUDES include/*.h)
file(GLOB_RECURSE SRC src/*.cpp)

include_directories(include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

################################
# Build targets
################################

add_library(mindbw
    ${SRC}
    ${INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR}/include/mindbw/cpp/DataTypes.h)

target_link_libraries(mindbw
    ${KIZHI_LIBRARIES}
    ${SQLITE3_LIBRARIES}
    ${ZEPHYR_LIBRARIES})

add_custom_command(OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/include/mindbw/cpp/DataTypes.h
    COMMAND python
    ARGS $ENV{PROJECT_SCRIPT_DIR}/include/generate-data.py
    ${CMAKE_CURRENT_LIST_DIR}/xml/data.xml
    ${CMAKE_CURRENT_BINARY_DIR}/include/mindbw)

################################
# Export library
################################
set(MINDBW_INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/include)

export_project(
    NAME mindbw
    INCLUDES ${MINDBW_INCLUDES}
    LIBS mindbw
    VERSION ${DB_VERSION}
)

export_java_project(
    NAME mindbw-java
    INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/include/mindbw/java
)

################################
# Unit tests
################################
add_unit_tests(NAME mindbw)

add_custom_target(mindbw-test-copy
    rm -f ${CMAKE_CURRENT_BINARY_DIR}/test/test.db
    COMMAND ${CMAKE_CURRENT_LIST_DIR}/scripts/sqlite3-execute
    ${CMAKE_CURRENT_BINARY_DIR}/test/test.db
    ${CMAKE_CURRENT_LIST_DIR}/test/schema.sql
)
add_dependencies(mindbw-test mindbw-test-copy)

