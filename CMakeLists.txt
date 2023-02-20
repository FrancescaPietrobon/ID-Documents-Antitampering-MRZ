cmake_minimum_required(VERSION 3.23)

project(document_validator VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Verbose build option
if(NOT CMAKE_VERBOSE_MAKEFILE)
    set(CMAKE_VERBOSE_MAKEFILE off)
endif()

if(COVERAGE_ENABLED)
    message("Code Coverage enabled")
    set(CMAKE_CXX_FLAGS "-Wall -fprofile-arcs -ftest-coverage")
else()
    message("Code Coverage NOT enabled")
    set(CMAKE_CXX_FLAGS "-Wall")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fno-inline -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "-m64 -O3")

set(CMAKE_INSTALL_LIBDIR "lib/x86_64-linux-gnu")
set(CMAKE_INSTALL_INCLUDEDIR "include")

message(STATUS "Library installation path: ${CMAKE_INSTALL_LIBDIR}")
message(STATUS "Headers installation path: ${CMAKE_INSTALL_INCLUDEDIR}")

# ################
# Find packages #
# ################
find_package(OpenCV)
find_package(spdlog)
find_package(jsoncpp)
find_package(nlohmann_json)
find_package(GTest REQUIRED)
find_package(Protobuf REQUIRED)

# Common
set(COMMON_PATH "src/common" CACHE STRING "Common folder")
file(GLOB_RECURSE APP_COMMON "${COMMON_PATH}/*.h" "${COMMON_PATH}/*.cpp" "${COMMON_PATH}/*.hpp")
file(GLOB_RECURSE COMMON_HEADERS "${COMMON_PATH}/*.hpp" "${COMMON_PATH}/*.h")

# Base64
set(BASE64_PATH "src/base64" CACHE STRING "Base64 folder")
file(GLOB_RECURSE APP_BASE64 "${BASE64_PATH}/*.h" "${BASE64_PATH}/*.cpp" "${BASE64_PATH}/*.hpp")
file(GLOB_RECURSE BASE64_HEADERS "${BASE64_PATH}/*.hpp" "${BASE64_PATH}/*.h")

# ##########
# Ocr stub #
# ##########

set(OCR_PATH "src/ocr" CACHE STRING "Ocr stub folder")
file(GLOB_RECURSE APP_OCR "${OCR_PATH}/*.h" "${OCR_PATH}/*.cpp" "${OCR_PATH}/*.hpp")
file(GLOB_RECURSE OCR_HEADERS "${OCR_PATH}/*.hpp" "${OCR_PATH}/*.h")

add_library(document_validator_ocr SHARED
    ${APP_OCR}
    ${APP_BASE64}
    ${APP_COMMON}
)

target_sources(document_validator_ocr
    PUBLIC FILE_SET headers
    TYPE HEADERS
    BASE_DIRS "src" FILES
    ${OCR_HEADERS}
    ${BASE64_HEADERS}
    ${COMMON_HEADERS}
)

target_include_directories(document_validator_ocr PUBLIC
    ${OCR_PATH}
    ${BASE64_PATH}
    ${COMMON_PATH}
)

target_link_libraries(document_validator_ocr
    spdlog::spdlog
    jsoncpp_lib
    nlohmann_json::nlohmann_json
    opencv::opencv
)

install(TARGETS document_validator_ocr
    COMPONENT ocr
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FILE_SET headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/document_validator
)


# ############################
# Characters clustering stub #
# ############################

set(CHARACTERS_CLUSTERING_PATH "src/charactersClustering" CACHE STRING "CharactersClustering stub folder")
file(GLOB_RECURSE APP_CHARACTERS_CLUSTERING "${CHARACTERS_CLUSTERING_PATH}/*.h" "${CHARACTERS_CLUSTERING_PATH}/*.cpp" "${CHARACTERS_CLUSTERING_PATH}/*.hpp")
file(GLOB_RECURSE CHARACTERS_CLUSTERING_HEADERS "${CHARACTERS_CLUSTERING_PATH}/*.hpp" "${CHARACTERS_CLUSTERING_PATH}/*.h")

add_library(document_validator_characters_clustering SHARED
    ${APP_CHARACTERS_CLUSTERING}
    ${APP_BASE64}
    ${APP_COMMON}
)

target_sources(document_validator_characters_clustering
    PUBLIC FILE_SET headers
    TYPE HEADERS
    BASE_DIRS "src" FILES
    ${CHARACTERS_CLUSTERING_HEADERS}
    ${BASE64_HEADERS}
    ${COMMON_HEADERS}
)

target_include_directories(document_validator_characters_clustering PUBLIC
    ${CHARACTERS_CLUSTERING_PATH}
    ${BASE64_PATH}
    ${COMMON_PATH}
)

target_link_libraries(document_validator_characters_clustering
    spdlog::spdlog
    jsoncpp_lib
    opencv::opencv
)

install(TARGETS document_validator_characters_clustering
    COMPONENT characters_clustering
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FILE_SET headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/document_validator
)

#[===[

# ###################
# Antitampering MRZ #
# ###################

set(ANTITAMPERING_MRZ_PATH "src/antitampering/mrz" CACHE STRING "Antitampering MRZ")
file(GLOB_RECURSE APP_ANTITAMPERING_MRZ "${ANTITAMPERING_MRZ_PATH}/*.h" "${ANTITAMPERING_MRZ_PATH}/*.cpp" "${ANTITAMPERING_MRZ_PATH}/*.hpp")
file(GLOB_RECURSE ANTITAMPERING_MRZ_HEADERS "${ANTITAMPERING_MRZ_PATH}/*.hpp" "${ANTITAMPERING_MRZ_PATH}/*.h")

add_library(document_validator_antitampering_mrz SHARED
    ${APP_ANTITAMPERING_MRZ}
    ${APP_COMMON}
)

target_sources(document_validator_antitampering_mrz
    PUBLIC FILE_SET headers
    TYPE HEADERS
    BASE_DIRS "src" FILES
    ${ANTITAMPERING_MRZ_HEADERS}
    ${COMMON_HEADERS}
)

target_include_directories(document_validator_antitampering_mrz PUBLIC
    ${ANTITAMPERING_MRZ_PATH}
    ${COMMON_PATH}
)

target_link_libraries(document_validator_antitampering_mrz
    opencv::opencv
    spdlog::spdlog
    jsoncpp_lib
    nlohmann_json::nlohmann_json
)

install(TARGETS document_validator_antitampering_mrz
    COMPONENT antitampering_mrz
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FILE_SET headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/document_validator
)

]===]

# #################
# Print Utilities #
# #################

set(PRINT_PATH "src/print" CACHE STRING "Print")
file(GLOB_RECURSE APP_PRINT "${PRINT_PATH}/*.h" "${PRINT_PATH}/*.cpp" "${PRINT_PATH}/*.hpp")
file(GLOB_RECURSE PRINT_HEADERS "${PRINT_PATH}/*.hpp" "${PRINT_PATH}/*.h")

add_library(print SHARED
    ${APP_PRINT}
    ${APP_COMMON}
)

target_sources(print
    PUBLIC FILE_SET headers
    TYPE HEADERS
    BASE_DIRS "src" FILES
    ${PRINT_HEADERS}
    ${COMMON_HEADERS}
)

target_include_directories(print PUBLIC
    ${PRINT_PATH}
    ${COMMON_PATH}
)

target_link_libraries(print
    opencv::opencv
)

# ###################
# Standalone runner #
# ###################

add_executable(application src/main.cpp)

target_link_libraries(application
    #document_validator
    print
    document_validator_ocr
    document_validator_characters_clustering
    #document_validator_antitampering_mrz
)

# #######
# TESTS #
# #######

enable_testing()
include(GoogleTest)

# Unit Tests
set(TESTS_OCR_PATH "src/tests/testCases/unitTests/ocr/" CACHE STRING "Unit tests folder")
file(GLOB_RECURSE APP_TESTS_OCR "${TESTS_OCR_PATH}/*.h" "${TESTS_OCR_PATH}/*.cpp" "${TESTS_OCR_PATH}/*.hpp")

#set(TESTS_CHARACTERS_CLUSTERING_PATH "src/tests/testCases/unitTests/charactersClustering/" CACHE STRING "Unit tests folder")
#file(GLOB_RECURSE APP_TESTS_CHARACTERS_CLUSTERING "${TESTS_CHARACTERS_CLUSTERING_PATH}/*.h" "${TESTS_CHARACTERS_CLUSTERING_PATH}/*.cpp" "${TESTS_CHARACTERS_CLUSTERING_PATH}/*.hpp")

#set(TESTS_ANTITAMPERING_PATH "src/tests/testCases/unitTests/antitampering/" CACHE STRING "Unit tests folder")
#file(GLOB_RECURSE APP_TESTS_ANTITAMPERING "${TESTS_ANTITAMPERING_PATH}/*.h" "${TESTS_ANTITAMPERING_PATH}/*.cpp" "${TESTS_ANTITAMPERING_PATH}/*.hpp")

# Test data
file(COPY "src/tests/testData" DESTINATION ".")

# Test document_validator_ocr
add_executable(TestOcr
    src/tests/main.cpp
    ${APP_TESTS_OCR}
)

target_include_directories(TestOcr PUBLIC "src/tests")

target_link_libraries(TestOcr
    document_validator_ocr
    GTest::gmock
)

gtest_discover_tests(TestOcr)

#[===[

# Test document_validator_characters_clustering
add_executable(TestCharactersClustering
    src/tests/main.cpp
    ${APP_TESTS_CHARACTERS_CLUSTERING}
)

target_include_directories(TestCharactersClustering PUBLIC "src/tests")

target_link_libraries(TestCharactersClustering
    document_validator_characters_clustering
    GTest::gmock
)

gtest_discover_tests(TestCharactersClustering)

]===]


#[===[

# Test document_validator_antitampering_mrz
add_executable(TestAntitampering
    src/tests/main.cpp
    ${APP_TESTS_ANTITAMPERING}
)

target_include_directories(TestAntitampering PUBLIC "src/tests")

target_link_libraries(TestAntitampering
    document_validator_antitampering_mrz
    GTest::gmock
)

gtest_discover_tests(TestAntitampering)

]===]