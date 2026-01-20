cmake_minimum_required(VERSION 3.16)
project(RayTracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Boost headers
find_package(Boost REQUIRED)

add_executable(raytracer
    src/main.cpp
    src/schwarzschildMetric.cpp
)

target_include_directories(raytracer
    PRIVATE
        ${Boost_INCLUDE_DIRS}
        include
)
