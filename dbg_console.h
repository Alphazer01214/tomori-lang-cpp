//
// Created by Alphazer01214 on 2025/12/24.
//
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "core/Interpreter.h"
#include "core/Lexer.h"
#include "core/Parser.h"

inline void dbg_console() {
    Interpreter interpreter;
    Lexer lexer(true);

    std::string source_buffer; // 用于累积多行输入
    int block_depth = 0;       // 跟踪 if/loop/function 的嵌套深度

    std::cout << "TomoriLang REPL (type 'exit' to quit)" << std::endl;

    while (true) {
        std::cout << (block_depth > 0 ? std::string(block_depth * 3, '.') : ">>> ");

        std::string line;
        if (!std::getline(std::cin, line)) break; // 处理 EOF (Ctrl+D/Z)

        if (line == "exit") break;

        try {
            auto temp_tokens = lexer.tokenize(line, false);
            for (const auto& t : temp_tokens) {
                if (t.type == TokenType::Then) {
                    block_depth++;
                }
                if (t.type == TokenType::End) {
                    block_depth--;
                }
            }
        } catch (...) {
            // 忽略词法错误（比如字符串没闭合），留给后面完整解析时报错
        }
        source_buffer += line + "\n";
        if (block_depth > 0) continue;
        if (block_depth < 0) {
            std::cerr << "Syntax Error: Unmatched 'end'" << std::endl;
            block_depth = 0;
            source_buffer.clear();
            continue;
        }
        if (source_buffer.find_first_not_of(" \t\n\r") == std::string::npos) {
            source_buffer.clear();
            continue;
        }
        try {
            auto tokens = lexer.tokenize(source_buffer, true, false);
            Parser parser(tokens, false);
            auto statements = parser.parse();
            if (!statements.empty()) {
                interpreter.interpret(statements);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown Error occurred." << std::endl;
        }
        source_buffer.clear();
        block_depth = 0;
    }
}