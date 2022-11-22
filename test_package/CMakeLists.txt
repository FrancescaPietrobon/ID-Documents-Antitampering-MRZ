cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-Wall")

#set(BINARY ${CMAKE_PROJECT_NAME}_tst)

find_package(GTest REQUIRED)
find_package(Boost REQUIRED)
find_package(OpenCV REQUIRED)
find_package(jsoncpp)
find_package(spdlog)
#find_package(antitampering_mrz CONFIG REQUIRED)

option(gtest_disable_pthreads "Disable uses of pthreads in gtest." OFF)

# Test suites
set(TEST_SUITES_PATH "src/testCases" CACHE STRING "Test cases folder")
include_directories(${TEST_SUITES_PATH})
file(GLOB_RECURSE TEST_SUITES "${TEST_SUITES_PATH}/*.h" "${TEST_SUITES_PATH}/*.cpp" "${TEST_SUITES_PATH}/*.hpp")

include_directories("src")

# Test data
file(COPY "src/testData" DESTINATION ".")

add_executable(TestRunner
    src/main.cpp
    ${TEST_SUITES}
)

target_link_libraries(TestRunner PRIVATE
    ${APP_ANTITAMPERING_MRZ}
    GTest::gtest
    GTest::gmock
    Boost::boost
    ${OpenCV_LIBS}
    jsoncpp_lib
    spdlog::spdlog
    #Boost::filesystem
)