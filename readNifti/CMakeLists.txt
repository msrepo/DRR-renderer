cmake_minimum_required(VERSION 3.5)

project(readNifti LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

include_directories(../argparse)

add_executable(readNifti main.cpp niftireader.cpp)
target_link_libraries(readNifti ${ITK_LIBRARIES} stdc++fs)
