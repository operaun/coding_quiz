cmake_minimum_required(VERSION 3.1.0)

# For Relase Mode
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# For Debugging purpose
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0")
#add_definitions(-DENABLE_FILE_INPUT_UNITTEST) # For unittest input file

add_executable(vote_program vote_program.cc)
