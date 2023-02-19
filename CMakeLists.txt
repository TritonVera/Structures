cmake_minimum_required(VERSION 3.24)
set(TOOLCHAIN_DIR "toolchain dir name")
set(CMAKE_TOOLCHAIN_FILE "cmake toolchain file")

project(Structures LANGUAGES CXX VERSION 1.0)
set(CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

# Structure Library
add_library(Structures STATIC 
    byteArray.cpp
    digitalFilter.cpp)

# Structure Includes
target_include_directories(Structures PRIVATE
    ${SYSTEM_INCLUDE_DIR} 
    ${K1921VK01T_INCLUDE_DIR}
    ${LIBRARY_INCLUDE_DIR})

target_compile_options(Structures PRIVATE "-fconcepts" "-std=c++20")

# Install package
install(
    TARGETS Structures
    ARCHIVE 
    DESTINATION "${TOOLCHAIN_DIR}/lib")

install(
    FILES 
        queue.h 
        queueNode.h 
        stackNode.h 
        stack.h 
        byteArray.hpp
        dictionary.hpp 
        functionWrapper.hpp 
        cycleArray.hpp
        digitalFilter.hpp
    DESTINATION "${TOOLCHAIN_DIR}/include/structures")

