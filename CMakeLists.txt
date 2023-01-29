cmake_minimum_required(VERSION 3.9)

project(antitampering_mrz VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)

set(BINARY ${CMAKE_PROJECT_NAME})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug) #Release
endif()

set(MRZ_ANTITAMPERING_PATH "antitampering_mrz/src" CACHE STRING "MrzAntitampering stub folder")
file(GLOB_RECURSE APP_ANTITAMPERING_MRZ "${MRZ_ANTITAMPERING_PATH}/*.h" "${MRZ_ANTITAMPERING_PATH}/*.cpp" "${MRZ_ANTITAMPERING_PATH}/*.hpp")
file(GLOB_RECURSE ANTITAMPERING_MRZ_HEADERS "${MRZ_ANTITAMPERING_PATH}/*.hpp" "${MRZ_ANTITAMPERING_PATH}/*.h")

add_library(antitampering_mrz STATIC
    ${APP_ANTITAMPERING_MRZ}
)

find_package(OpenCV REQUIRED)
find_package(Boost)
find_package(jsoncpp)
find_package(spdlog REQUIRED)


include_directories( ${OpenCV_INCLUDE_DIRS} )
#target_link_libraries(OCR ${OpenCV_LIBS})

target_sources(antitampering_mrz
    PUBLIC FILE_SET headers
    TYPE HEADERS
    #BASE_DIRS ${PROJECT_SOURCE_DIR} FILES
    BASE_DIRS "antitampering_mrz" FILES
    ${ANTITAMPERING_MRZ_HEADERS}
)

target_include_directories(antitampering_mrz PUBLIC
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(antitampering_mrz PRIVATE
    #Boost::boost
    spdlog::spdlog
    jsoncpp_lib
    #nlohmann_json::nlohmann_json
    ${OpenCV_LIBS}
)

add_executable(app)
#target_sources(app PRIVATE antitampering_mrz/src/antitampering/mrz/antitamperingMrzFactory.cpp)
#target_sources(app PRIVATE antitampering_mrz/src/appFactory.cpp)
target_sources(app PRIVATE antitampering_mrz/src/ocr/OcrFactory.cpp)

target_link_libraries(app PRIVATE antitampering_mrz)





#add_subdirectory(antitampering_mrz)
#add_subdirectory(test_package)

#if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
#    add_subdirectory(test_package)
#endif ()

#enable_testing()


############################################
#                TESTS                     #
############################################

enable_testing()


# Unit Tests
set(UNIT_TESTS_PATH "antitampering_mrz/src/tests/testCases/unitTests" CACHE STRING "Unit tests folder")
file(GLOB_RECURSE APP_UNIT_TESTS "${UNIT_TESTS_PATH}/*.h" "${UNIT_TESTS_PATH}/*.cpp" "${UNIT_TESTS_PATH}/*.hpp")

# EndToEnd Tests
#set(END_TO_END_TESTS_PATH "antitampering_mrz/src/tests/testCases/endtoendTests" CACHE STRING "End to end tests folder")
#file(GLOB_RECURSE APP_END_TO_END_TESTS "${END_TO_END_TESTS_PATH}/*.h" "${END_TO_END_TESTS_PATH}/*.cpp" "${END_TO_END_TESTS_PATH}/*.hpp")

# Test data
file(COPY "antitampering_mrz/src/tests/testData" DESTINATION ".")

add_executable(TestExecutable 
    antitampering_mrz/src/tests/main.cpp 
    ${APP_UNIT_TESTS}
    ${APP_END_TO_END_TESTS}
)

target_include_directories(TestExecutable PUBLIC "antitampering_mrz/src/tests")

find_package(GTest REQUIRED)
find_package(Boost REQUIRED)

target_link_libraries(TestExecutable
    antitampering_mrz
    Boost::boost
    #Boost::filesystem
    GTest::gmock
    ${OpenCV_LIBS}
)

include(GoogleTest)

gtest_discover_tests(TestExecutable)
