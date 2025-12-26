cmake_minimum_required(VERSION 4.1)
project(tomori_lang_cpp)

set(CMAKE_CXX_STANDARD 26)

add_executable(tomori_lang_cpp main.cpp
        core/Lexer.cpp
        core/Lexer.h
        core/AST.h
        core/Parser.cpp
        core/Parser.h
        core/Interpreter.cpp
        core/Interpreter.h
        core/Environment.cpp
        core/Environment.h
        core/Define.h
        core/Value.h
        dbg_console.h
        core/Builtin.h
)
