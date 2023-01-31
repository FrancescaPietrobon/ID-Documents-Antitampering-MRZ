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

set(CMAKE_INSTALL_LIBDIR "lib/x86_64-linux-gnu")
set(CMAKE_INSTALL_INCLUDEDIR "include")

# ################
# Find packages #
# ################
find_package(OpenCV)
find_package(spdlog)
find_package(jsoncpp)
find_package(nlohmann_json)
find_package(GTest REQUIRED)

# Base64
set(BASE64_PATH "src/base64" CACHE STRING "Base64 folder")
file(GLOB_RECURSE APP_BASE64 "${BASE64_PATH}/*.h" "${BASE64_PATH}/*.cpp" "${BASE64_PATH}/*.hpp")
file(GLOB_RECURSE BASE64_HEADERS "${BASE64_PATH}/*.hpp" "${BASE64_PATH}/*.h")

#[===[

# ######
# Core #
# ######

add_library(document_validator SHARED
    ${APP_BASE64}
)

target_sources(document_validator
    PUBLIC FILE_SET headers
    TYPE HEADERS
    BASE_DIRS "src" FILES
    ${BASE64_HEADERS}
)

target_include_directories(document_validator PUBLIC
    ${BASE64_PATH}
)

target_link_libraries(document_validator
    opencv::opencv
    spdlog::spdlog
    jsoncpp_lib
)

install(TARGETS document_validator
    COMPONENT core
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FILE_SET headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/document_validator
)
]===]

# #####
# Ocr #
# #####

set(OCR_PATH "src/ocr" CACHE STRING "Ocr stub folder")
file(GLOB_RECURSE APP_OCR "${OCR_PATH}/*.h" "${OCR_PATH}/*.cpp" "${OCR_PATH}/*.hpp")
file(GLOB_RECURSE OCR_HEADERS "${OCR_PATH}/*.hpp" "${OCR_PATH}/*.h")

add_library(document_validator_ocr SHARED
    ${APP_OCR}
    ${APP_BASE64}
)

target_sources(document_validator_ocr
    PUBLIC FILE_SET headers
    TYPE HEADERS
    BASE_DIRS "src" FILES
    ${OCR_HEADERS}
    ${BASE64_HEADERS}
)

target_include_directories(document_validator_ocr PUBLIC
    ${OCR_PATH}
    ${BASE64_PATH}
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

#[===[

# ###########################
# Characters associator stub #
# ###########################

set(CHARACTERS_ASSOCIATOR_PATH "src/characters_associator" CACHE STRING "CharactersAssociator stub folder")
file(GLOB_RECURSE APP_CHARACTERS_ASSOCIATOR "${CHARACTERS_ASSOCIATOR_PATH}/*.h" "${CHARACTERS_ASSOCIATOR_PATH}/*.cpp" "${CHARACTERS_ASSOCIATOR_PATH}/*.hpp")
file(GLOB_RECURSE CHARACTERS_ASSOCIATOR_HEADERS "${CHARACTERS_ASSOCIATOR_PATH}/*.hpp" "${CHARACTERS_ASSOCIATOR_PATH}/*.h")

add_library(document_validator_characters_associator SHARED
    ${APP_CHARACTERS_ASSOCIATOR}
    ${APP_BASE64}
)

target_sources(document_validator_characters_associator
    PUBLIC FILE_SET headers
    TYPE HEADERS
    BASE_DIRS "src" FILES
    ${CHARACTERS_ASSOCIATOR_HEADERS}
    ${BASE64_HEADERS}
)

target_include_directories(document_validator_characters_associator PUBLIC
    ${CHARACTERS_ASSOCIATOR_PATH}
    ${BASE64_PATH}
)

target_link_libraries(document_validator_characters_associator
    spdlog::spdlog
    jsoncpp_lib
    opencv::opencv
)

install(TARGETS document_validator_characters_associator
    COMPONENT characters_associator
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FILE_SET headers DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/document_validator
)

]===]

# ###################
# Antitampering MRZ #
# ###################

set(ANTITAMPERING_MRZ_PATH "src/antitampering/mrz" CACHE STRING "Antitampering MRZ")
file(GLOB_RECURSE APP_ANTITAMPERING_MRZ "${ANTITAMPERING_MRZ_PATH}/*.h" "${ANTITAMPERING_MRZ_PATH}/*.cpp" "${ANTITAMPERING_MRZ_PATH}/*.hpp")
file(GLOB_RECURSE ANTITAMPERING_MRZ_HEADERS "${ANTITAMPERING_MRZ_PATH}/*.hpp" "${ANTITAMPERING_MRZ_PATH}/*.h")

add_library(document_validator_antitampering_mrz SHARED
    ${APP_ANTITAMPERING_MRZ}
    ${APP_BASE64}
)

target_sources(document_validator_antitampering_mrz
    PUBLIC FILE_SET headers
    TYPE HEADERS
    BASE_DIRS "src" FILES
    ${ANTITAMPERING_MRZ_HEADERS}
)

target_include_directories(document_validator_antitampering_mrz PUBLIC
    ${ANTITAMPERING_MRZ_PATH}
    ${DATA_PATH}
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


# ###################
# Standalone runner #
# ###################

add_executable(application src/main.cpp)

target_link_libraries(application
    #document_validator
    document_validator_ocr
    #document_validator_characters_associator
    document_validator_antitampering_mrz
)

# #######
# TESTS #
# #######

enable_testing()


# Unit Tests
set(UNIT_TESTS_PATH "src/tests/testCases/unitTests" CACHE STRING "Unit tests folder")
file(GLOB_RECURSE APP_UNIT_TESTS "${UNIT_TESTS_PATH}/*.h" "${UNIT_TESTS_PATH}/*.cpp" "${UNIT_TESTS_PATH}/*.hpp")

# Test data
file(COPY "src/tests/testData" DESTINATION ".")

add_executable(TestExecutable
    src/tests/main.cpp
    ${APP_UNIT_TESTS}
)

target_include_directories(TestExecutable PUBLIC "src/tests")

target_link_libraries(TestExecutable
    #document_validator
    document_validator_ocr
    #document_validator_characters_associator
    document_validator_antitampering_mrz
    GTest::gmock
)

include(GoogleTest)

gtest_discover_tests(TestExecutable)