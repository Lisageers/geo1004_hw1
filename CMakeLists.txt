cmake_minimum_required(VERSION 3.17)
project(hw1)

set(CMAKE_CXX_STANDARD 14)

include_directories(voxeliser)

add_executable(hw1
        voxeliser/main.cpp
        voxeliser/Point.h
        voxeliser/Rows.h
        voxeliser/VoxelGrid.h)
