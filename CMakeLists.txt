cmake_minimum_required(VERSION 3.22.1)
project(evtol)

set(CMAKE_CXX_STANDARD 11)

add_executable(evtol main.cpp
        Simulator.cpp
        Simulator.h)
