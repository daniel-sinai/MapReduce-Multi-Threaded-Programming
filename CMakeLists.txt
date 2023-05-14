cmake_minimum_required(VERSION 3.22)
project(ex3)

set(CMAKE_CXX_STANDARD 11)
include_directories(.)

add_executable(ex3
#        Atomic/atomicdemo.cpp
#        Atomic/atomicdemo_multivar.cpp
#        Barrier/Barrier.cpp
#        Barrier/Barrier.h
#        Barrier/barrierdemo.cpp
#        Sample Client/SampleClient.cpp
        MapReduceClient.h
        MapReduceFramework.cpp
        MapReduceFramework.h
        ThreadContext.cpp
        ThreadContext.h
        GlobalContext.cpp
        GlobalContext.h
        SampleClient.cpp
        MapReduce.cpp
        MapReduce.h
        Barrier.cpp
        Barrier.h)
