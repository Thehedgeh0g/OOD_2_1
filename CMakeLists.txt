cmake_minimum_required(VERSION 3.29)
project(OOD_2_1)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        OOD_2_1 2_1/main.cpp
        2_1/Observer.h
        2_1/WeatherData.h
)

add_executable(
        OOD_2_2 2_2/main.cpp
        2_2/Observer.h
        2_2/WeatherData.h
)

add_executable(
        OOD_2_3 2_3/main.cpp
        2_3/Observer.h
        2_3/WeatherData.h
)

add_executable(
        OOD_2_4 2_4/main.cpp
        2_4/Observer.h
        2_4/WeatherData.h
)

add_executable(
        OOD_2_5 2_5/main.cpp
        2_5/Observer.h
        2_5/WeatherData.h
)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(
        OOD_2_2Test 2_2/Test.cpp
        2_2/Observer.h
        2_2/WeatherData.h
)
target_link_libraries(OOD_2_2Test gtest gtest_main)
enable_testing()
add_test(NAME OOD_2_2Test COMMAND OOD_2_2Test)

add_executable(
        OOD_2_3Test 2_3/Test.cpp
        2_3/Observer.h
        2_3/WeatherData.h
)
target_link_libraries(OOD_2_3Test gtest gtest_main)
enable_testing()
add_test(NAME OOD_2_3Test COMMAND OOD_2_3Test)
