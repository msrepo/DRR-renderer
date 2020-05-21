project(DRRgenerator)
cmake_minimum_required(VERSION 2.8)
ADD_DEFINITIONS(
    -std=c++11
    -O3
)
set(STD_EXPERIMENTAL_FS "-lstdc++fs")

message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(Eigen3 REQUIRED)
include_directories( ${EIGEN3_INCLUDE_DIR} )


include_directories(argparse)

add_executable(${PROJECT_NAME} src/main.cpp src/DRRgenerator.cpp )
target_link_libraries( ${PROJECT_NAME}   ${OpenCV_LIBS}  Eigen3::Eigen stdc++fs)


