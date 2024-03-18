cmake_minimum_required(VERSION 3.22.1)
project(evtol)

set(CMAKE_CXX_STANDARD 11)

# Fetch and apply  Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)
include_directories(${gtest_SOURCE_DIR}/include)
include_directories($PROJECT_SOURCE_DIR)
add_subdirectory(tests) 

# clang-tidy TODO - move to use the config file later this is my boilerplate.
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=-*,cppcoreguidelines-*,-isystem")  # Customize checks
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()
add_test(NAME SimulatorTests COMMAND simulator_tests) # Assuming your test executable is 'my_tests'

add_library(evtol_core STATIC Aircraft.h Aircraft.cpp Simulator.h Simulator.cpp)
add_executable(evtol main.cpp)
target_link_libraries(evtol PRIVATE evtol_core)
target_link_libraries(evtol PRIVATE evtol_core)