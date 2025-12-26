//
// Created by Alphazer01214 on 2025/12/22.
//

#ifndef TOMORI_LANG_CPP_DEFINE_H
#define TOMORI_LANG_CPP_DEFINE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <initializer_list>
#include <utility>
#include <cctype>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <algorithm>

// Lexer / Parser
enum class TokenType {
    Identifier,   // 函数、变量名
    Number,
    String,
    Boolean,

    // 关键字
    If, ElseIf, Else, Then, End, Loop,
    Var, Function, Have, Return,
    Continue, Break,
    And, Or, Not, Is, Default, IsNot,
    Import,

    // 运算符
    Plus, Minus, Multiply, Divide, Modulo, Power,
    BitAnd, BitOr, BitXor, BitShiftLeft, BitShiftRight,
    Assign,   // =
    Less, LessEqual, Greater, GreaterEqual,

    //
    LeftParen, RightParen,  // ()
    Semicolon,   // ;
    Comma,      // ,
    Eof
};

struct Token {
    TokenType type;
    std::string token;
    int line;
};

static std::unordered_map<std::string, TokenType> keywords = {
    {"if", TokenType::If},
    {"elseif", TokenType::ElseIf},
    {"else", TokenType::Else},
    {"then", TokenType::Then},
    {"end", TokenType::End},
    {"loop", TokenType::Loop},
    {"var", TokenType::Var},
    {"function", TokenType::Function},
    {"have", TokenType::Have},
    {"return", TokenType::Return},
    {"continue", TokenType::Continue},
    {"break", TokenType::Break},
    {"default", TokenType::Default},
    {"import", TokenType::Import},

    {"and", TokenType::And},
    {"or", TokenType::Or},
    {"not", TokenType::Not},
    {"is", TokenType::Is},
    {"isnot", TokenType::IsNot},

    {"true", TokenType::Boolean},
    {"false", TokenType::Boolean},
};

static std::unordered_map<TokenType, std::string> reverse_keywords = {
    // 基础类型
    {TokenType::Identifier, "Identifier"},
    {TokenType::Number, "Number"},
    {TokenType::String, "String"},
    {TokenType::Boolean, "Boolean"},

    // 关键字
    {TokenType::If, "If"},
    {TokenType::ElseIf, "ElseIf"},
    {TokenType::Else, "Else"},
    {TokenType::Then, "Then"},
    {TokenType::End, "End"},
    {TokenType::Loop, "Loop"},
    {TokenType::Var, "Var"},
    {TokenType::Function, "Function"},
    {TokenType::Have, "Have"},
    {TokenType::Return, "Return"},
    {TokenType::Continue, "Continue"},
    {TokenType::Break, "Break"},
    {TokenType::And, "And"},
    {TokenType::Or, "Or"},
    {TokenType::Not, "Not"},
    {TokenType::Is, "Is"},
    {TokenType::IsNot, "IsNot"},
    {TokenType::Default, "Default"},

    // 运算符
    {TokenType::Plus, "Plus"},
    {TokenType::Minus, "Minus"},
    {TokenType::Multiply, "Multiply"},
    {TokenType::Divide, "Divide"},
    {TokenType::Modulo, "Modulo"},
    {TokenType::Power, "Power"},
    {TokenType::BitAnd, "BitAnd"},
    {TokenType::BitOr, "BitOr"},
    {TokenType::BitXor, "BitXor"},
    {TokenType::BitShiftLeft, "BitShiftLeft"},
    {TokenType::BitShiftRight, "BitShiftRight"},
    {TokenType::Assign, "Assign"},
    {TokenType::Less, "Less"},
    {TokenType::LessEqual, "LessEqual"},
    {TokenType::Greater, "Greater"},
    {TokenType::GreaterEqual, "GreaterEqual"},

    // 分隔符
    {TokenType::LeftParen, "LeftParen"},
    {TokenType::RightParen, "RightParen"},
    {TokenType::Semicolon, "Semicolon"},
    {TokenType::Comma, "Comma"},

    // 特殊
    {TokenType::Eof, "Eof"}
};

// enum class ControlSignal {
//     None,
//     Break,
//     Continue,
//     Return
// };
//
// enum class ErrorSignal {
//     None,
//     RuntimeError,
//     CompileError
// };


#endif //TOMORI_LANG_CPP_DEFINE_H