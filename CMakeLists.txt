cmake_minimum_required(VERSION 3.22)

set(CPM_DOWNLOAD_VERSION 0.35.5)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()

include(${CPM_DOWNLOAD_LOCATION})

project(dbatk)

add_subdirectory(../CoreMUD coremud_build)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)


file(GLOB_RECURSE DBATK_INCLUDE include/*.h)
file(GLOB_RECURSE DBATK_SRC src/*.cpp)

SET(DBATK_INCLUDE_DIRS
        include
        ${COREMUD_INCLUDE_DIRS}
)

set(DBATK_LINK_LIBRARIES
        ${COREMUD_LINK_LIBRARIES}
        coremud
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