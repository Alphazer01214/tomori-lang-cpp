cmake_minimum_required(VERSION 4.1)
project(tomori_lang_cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(tomori_lang_cpp main.cpp
        core/Lexer.cpp
        core/Lexer.h
        core/AST.h
        core/Parser.cpp
        core/Parser.h)
