cmake_minimum_required(VERSION 3.9)

project(OCR VERSION 1.0)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release) #Debug
endif()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_EXPORT_COMPILE_COMMANDS)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

add_library(Document src/Document.cpp)
add_library(Boxes src/Boxes.cpp)
add_library(ModelBoxes src/ModelBoxes.cpp)
add_library(XMLBoxes src/XMLBoxes.cpp)
add_library(DBSCAN src/DBSCAN.cpp)
add_library(Character src/Character.cpp)
add_library(Fields src/Fields.cpp)
add_library(Field src/Field.cpp)
add_library(MRZ src/MRZ.cpp)
add_library(Dictionary src/Dictionary.cpp)
add_library(Anchors src/Anchors.cpp)
add_library(utilities src/utilities.cpp)
add_library(pugixml include/pugixml.cpp)

find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(OCR main.cpp)

target_link_libraries(OCR ${OpenCV_LIBS})
target_link_libraries(OCR Document)
target_link_libraries(OCR Boxes)
target_link_libraries(OCR ModelBoxes)
target_link_libraries(OCR XMLBoxes)
target_link_libraries(OCR DBSCAN)
target_link_libraries(OCR Character)
target_link_libraries(OCR Fields)
target_link_libraries(OCR Field)
target_link_libraries(OCR MRZ)
target_link_libraries(OCR Dictionary)
target_link_libraries(OCR Anchors)
target_link_libraries(OCR utilities)
target_link_libraries(OCR pugixml)