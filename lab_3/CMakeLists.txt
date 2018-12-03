cmake_minimum_required(VERSION 3.12)
project(lab3_OC)

set(CMAKE_CXX_STANDARD 17)
link_libraries(stdc++fs)
add_executable(lab3_OC main.cpp Action.cpp Action.h view.h view.cpp AsyncCopy.cpp AsyncCopy.h)