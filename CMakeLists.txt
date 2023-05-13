cmake_minimum_required(VERSION 3.25)
project(cs377_lawfirm_scheduling_simulation)

set(CMAKE_CXX_STANDARD 17)

add_executable(cs377_lawfirm_scheduling_simulation src/main.cpp include/scheduling.h src/scheduling.cpp)
