//
// Created by Alphazer01214 on 2025/12/24.
//
#include "core/Lexer.h"
#include "core/Parser.h"

inline void dbg_console() {
    std::string source, line;
    while (std::getline(std::cin, line)) {
        source += line + '\n';
    }

    Lexer lexer(source, true);
    auto tokens = lexer.tokenize();
    Parser parser(tokens, true);

    // 不要单独调用 parse_expression()
    auto statements = parser.parse();
    for (auto statement : statements) {
        statement->print();
    }
}
