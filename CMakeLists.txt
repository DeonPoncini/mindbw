cmake_minimum_required(VERSION 2.8)
project(libdb)

################################
# Compiler setup
################################

add_definitions(-Wall)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${CMAKE_SOURCE_DIR}/../cmake/modules/")

################################
# Third party libraries
################################
find_package(SQLite3 REQUIRED)

include_directories(${SQLITE3_INCLUDE_DIR})

################################
# Locate source
################################
file(GLOB_RECURSE INCLUDES include/*.h)
file(GLOB_RECURSE SRC src/*.cpp)

include_directories(include)
include_directories(include/db)

################################
# Build targets
################################

add_library(db SHARED
    ${SRC}
    ${INCLUDES})

target_link_libraries(db
    ${SQLITE3_LIBRARIES})
