cmake_minimum_required(VERSION 3.16)
project(PetHotel CXX)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC "src/*.cpp")
include_directories(include)
add_executable(pet_hotel ${SRC} main.cpp)
