cmake_minimum_required(VERSION 3.22)

set(CPM_DOWNLOAD_VERSION 0.35.5)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()

include(${CPM_DOWNLOAD_LOCATION})

project(dbatk)

CPMAddPackage("gh:fmtlib/fmt#9.1.0")
CPMAddPackage("gh:robinlinden/libsodium-cmake#master")
CPMAddPackage("gh:SRombauts/SQLiteCpp#3.3.0")
CPMAddPackage("gh:skypjack/entt#v3.11.1")

find_package(SQLite3 REQUIRED)
find_package(Boost REQUIRED COMPONENTS system program_options regex)

add_subdirectory(../CoreMUD coremud_build)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)


file(GLOB_RECURSE DBATK_INCLUDE include/*.h)
file(GLOB_RECURSE DBATK_SRC src/*.cpp)

SET(DBATK_INCLUDE_DIRS
        include
        ${coremud_SOURCE_DIR}/include
        ${SQLiteCpp_SOURCE_DIR}/include
        ${boost_SOURCE_DIR}
        ${sodium_SOURCE_DIR}/libsodium/src/libsodium/include
        ${FMT_SOURCE_DIR}/include
        ${entt_SOURCE_DIR}/single_include
)

set(DBATK_LINK_LIBRARIES
        ${SQLite3_LIBRARIES}
        SQLiteCpp
        ${Boost_LIBRARIES}
        coremud
        sodium
        fmt::fmt
)

# this is the core library we're making.
add_library(dbatk ${DBATK_INCLUDE} ${DBATK_SRC})
target_link_libraries(dbatk ${DBATK_LINK_LIBRARIES})
target_include_directories(dbatk PUBLIC ${DBATK_INCLUDE_DIRS})
# everything we make will link to it.

add_executable(dbat apps/dbat.cpp)
target_link_libraries(dbat dbatk ${DBATK_LINK_LIBRARIES})
target_include_directories(dbat PUBLIC ${DBATK_INCLUDE_DIRS})

add_executable(dbimport apps/dbimport.cpp)
target_link_libraries(dbimport dbatk ${DBATK_LINK_LIBRARIES})
target_include_directories(dbimport PUBLIC ${DBATK_INCLUDE_DIRS})