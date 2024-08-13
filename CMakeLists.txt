cmake_minimum_required(VERSION 3.5)
include(FetchContent)

project(tokenizer.cpp)
SET(TARGET tokenizer)

include(CheckIncludeFileCXX)
# CPP version
check_include_file_cxx(any HAS_ANY)
check_include_file_cxx(string_view HAS_STRING_VIEW)
check_include_file_cxx(coroutine HAS_COROUTINE)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

OPTION(BUILD_SHARED_LIBS "Build shared libraries." OFF)

add_subdirectory(cpp-tiktoken)
add_subdirectory(sentencepiece)

if(BUILD_SHARED_LIBS)
add_library(${TARGET} SHARED 
    tokenizer.cc
)
else()
add_library(${TARGET} 
    tokenizer.cc
)
endif()

target_compile_features(${TARGET} PUBLIC cxx_std_17)