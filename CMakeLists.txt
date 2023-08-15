cmake_minimum_required(VERSION 3.20)
project(BDE VERSION 1.0)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

set(
    SRC_FILES
    ./core/main.cxx
    ./core/include/ArgumentParser.cxx
    ./core/include/TargetFile.cxx
    ./core/include/ExtractionEngine.cxx
    ./core/include/types/DataQueue.cxx
)

add_executable(
    bde
    ${SRC_FILES}
)

target_include_directories(
    bde
    PUBLIC "${CMAKE_SOURCE_DIR}/core/include"
)