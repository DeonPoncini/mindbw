cmake_minimum_required(VERSION 2.8)
project(libdb)

set(DB_VERSION "0.0.1")

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

################################
# Export library
################################

export_project(
    NAME libdb
    INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include
    LIBS db
    VERSION ${DB_VERSION}
)

