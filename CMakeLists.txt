cmake_minimum_required(VERSION 3.5)

project(DRRgenerator LANGUAGES CXX)

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

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})


include_directories(argparse)
include_directories(readNifti/include)
add_subdirectory(readNifti)

add_executable(DRRgenerator src/main.cpp  src/DRRgenerator.cpp)
target_link_libraries( DRRgenerator  ${ITK_LIBRARIES} ${OpenCV_LIBS}  Eigen3::Eigen stdc++fs)


