cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra -Wpedantic -Werror")

## generate Makefile in source directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

##########################
### Set up environment ###
##########################
set(PROJECT_NAME lawfirm_scheduling_simulation)
project(${PROJECT_NAME})
include_directories(include) # header files
add_library(scheduling_lib src/scheduling.cpp)

# fetch googletest
include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

####################################
### Generate the main executable ###
####################################
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} scheduling_lib)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

####################################
### Generate the test executable ###
####################################
add_executable(${PROJECT_NAME}_test tests/test.cpp)
target_link_libraries(${PROJECT_NAME}_test scheduling_lib gtest_main)
add_test(NAME test COMMAND ${PROJECT_NAME}_test)
set_target_properties(${PROJECT_NAME}_test PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)