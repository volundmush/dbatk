cmake_minimum_required(VERSION 3.17)

set(CPM_DOWNLOAD_VERSION 0.34.0)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()

include(${CPM_DOWNLOAD_LOCATION})

project(dbatk)

CPMAddPackage(
        NAME ringnet
        GITHUB_REPOSITORY volundmush/ringnet
        GIT_TAG main
)

CPMAddPackage(
        NAME ringmud
        GITHUB_REPOSITORY volundmush/ringmud
        GIT_TAG main
)

CPMAddPackage(
        NAME asio
        GITHUB_REPOSITORY chriskohlhoff/asio
        GIT_TAG asio-1-21-0
)

CPMAddPackage(
        NAME json
        GITHUB_REPOSITORY nlohmann/json
        VERSION 3.10.4
)

CPMAddPackage(
        NAME eff_random
        GITHUB_REPOSITORY effolkronium/random
        VERSION 1.4.0
)

CPMAddPackage(
        NAME entt
        GITHUB_REPOSITORY skypjack/entt
        VERSION 3.8.1
)

CPMAddPackage(
        NAME fmt
        GITHUB_REPOSITORY fmtlib/fmt
        GIT_TAG 8.0.1
)

CPMAddPackage(
        name sqlitepp
        GITHUB_REPOSITORY mpaland/sqlitepp
        GIT_TAG 1.6.0
)

CPMAddPackage(
        name argh
        GITHUB_REPOSITORY adishavit/argh
        VERSION 1.3.1
)

file(GLOB DBATK_INCLUDE src/*.h)
file(GLOB DBATK_SRC src/*.cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "-fpermissive")

include_directories(PUBLIC
        include/dbatk
        ${asio_SOURCE_DIR}/asio/include
        ${entt_SOURCE_DIR}/single_include
        ${json_SOURCE_DIR}/single_include
        ${fmt_SOURCE_DIR}/include
        ${eff_random_SOURCE_DIR}/include
        ${ringnet_SOURCE_DIR}/include
        ${ringmud_SOURCE_DIR}/include
        ${sqlitepp_SOURCE_DIR}
        ${argh_SOURCE_DIR}
)

link_libraries(fmt::fmt ringnet ringmud)

find_library(LIBBSD NAMES bsd libbsd)

if(${LIBBSD})
    link_libraries(${LIBBSD})
    add_compile_definitions(HAVE_LIBBSD)
endif()

# this is the core library we're making.
add_library(dbatk ${DBATK_INCLUDE} ${DBATK_SRC})
# everything we make will link to it.
link_libraries(dbatk)
target_compile_definitions(dbatk PUBLIC USING_CMAKE=1 CIRCLE_UNIX=1 POSIX=1)

add_executable(circle apps/circle.cpp)

SET(dbatk_link ${CMAKE_INSTALL_PREFIX}/bin/)
