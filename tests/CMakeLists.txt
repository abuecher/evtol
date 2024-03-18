add_executable(simulator_tests SimulatorTest.cpp)
target_link_libraries(simulator_tests PRIVATE evtol_core gtest gtest_main) 
