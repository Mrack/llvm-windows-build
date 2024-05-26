cmake_minimum_required(VERSION 3.6)
project(LLVMHelloPass)

set(CMAKE_CXX_STANDARD 20)

#set(LLVM_DIR E:\\llvm-12.0.1\\lib\\cmake\\llvm)

find_package(LLVM REQUIRED CONFIG)

list(APPEND CMAKE_MODULE_PATH ${LLVM_DIR})

include(AddLLVM)

add_definitions(${LLVM_DEFINITIONS})

include_directories(${LLVM_INCLUDE_DIRS})

link_directories(${LLVM_LIBRARY_DIRS})

set(LLVM_LINK_COMPONENTS Core Support)

set(SOURCES Hello.cpp)

add_llvm_library(${PROJECT_NAME} SHARED ${SOURCES})

if (${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo)
    target_compile_options(${PROJECT_NAME} PRIVATE "/Zi /Ob0 /Od /RTC1")
endif()