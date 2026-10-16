cmake_minimum_required(VERSION 4.3)
project(PWRRT_ZADANIA)

set(CMAKE_CXX_STANDARD 20)

add_executable(Zadanie1 zadanie1/main.cpp
        Includes/sPoint.h
        zadanie1/PointsSolver.cpp
        zadanie1/PointsSolver.h
        Includes/mMath.h
        Includes/mMath.cpp
        Includes/sRecord.h)
add_executable(Zadanie2 zadanie2/main.cpp
        Includes/sRecord.h
        zadanie2/SensorFusion.h
        zadanie2/SensorFusion.cpp
)