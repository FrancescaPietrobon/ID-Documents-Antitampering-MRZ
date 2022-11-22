cmake_minimum_required(VERSION 3.9)

project(antitampering_mrz VERSION 1.0)

set(BINARY ${CMAKE_PROJECT_NAME})

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug) #Release
endif()

file(GLOB_RECURSE APP_ANTITAMPERING_MRZ "${PROJECT_SOURCE_DIR}/*.h" "${PROJECT_SOURCE_DIR}/AntitamperingMrz/*.cpp" "${PROJECT_SOURCE_DIR}/*.hpp")
file(GLOB_RECURSE ANTITAMPERING_MRZ_HEADERS "${PROJECT_SOURCE_DIR}/*.hpp" "${PROJECT_SOURCE_DIR}/*.h")

add_library(antitampering_mrz STATIC
    ${APP_ANTITAMPERING_MRZ}
)

find_package(OpenCV REQUIRED)
find_package(Boost)
find_package(spdlog)
find_package(jsoncpp)

#include_directories( ${OpenCV_INCLUDE_DIRS} )
#target_link_libraries(OCR ${OpenCV_LIBS})

target_sources(antitampering_mrz
    PUBLIC FILE_SET headers
    TYPE HEADERS
    BASE_DIRS ${PROJECT_SOURCE_DIR} FILES
    ${ANTITAMPERING_MRZ_HEADERS}
)

target_include_directories(antitampering_mrz PUBLIC
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(antitampering_mrz
    #Boost::boost
    spdlog::spdlog
    jsoncpp_lib
    #nlohmann_json::nlohmann_json
    ${OpenCV_LIBS}
)

add_executable(app)
target_sources(app PRIVATE antitamperingMrzFactory.cpp)

target_link_libraries(app PRIVATE antitampering_mrz)