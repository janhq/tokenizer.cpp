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

add_subdirectory(cpp-tiktoken)
add_subdirectory(sentencepiece)

#TODO(sang) shared or static as option
add_library(${TARGET} SHARED 
    tokenizer.cc
)

target_compile_features(${TARGET} PUBLIC cxx_std_17)