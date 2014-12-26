cmake_minimum_required(VERSION 2.8)
project(mindbw)

set(DB_VERSION "0.0.1")

################################
# Third party libraries
################################
find_package(SQLite3 REQUIRED)

find_package(zephyr)

include_directories(${SQLITE3_INCLUDE_DIR})
include_directories(${ZEPHYR_INCLUDE_DIRS})

################################
# Locate source
################################
file(GLOB_RECURSE INCLUDES include/*.h)
file(GLOB_RECURSE SRC src/*.cpp)

include_directories(include)

################################
# Build targets
################################

add_library(mindbw
    ${SRC}
    ${INCLUDES})

target_link_libraries(mindbw
    ${SQLITE3_LIBRARIES}
    ${ZEPHYR_LIBRARIES})

################################
# Export library
################################

export_project(
    NAME mindbw
    INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include
    LIBS mindbw
    VERSION ${DB_VERSION}
)

