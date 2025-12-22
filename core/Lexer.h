//
// Created by Alphazer01214 on 2025/12/20.
//

#ifndef TOMORI_LANG_CPP_LEXER_H
#define TOMORI_LANG_CPP_LEXER_H
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cctype>
#include <iomanip>
#include <utility>


enum class TokenType {
    Identifier,   // 函数、变量名
    Number,
    String,
    Boolean,

    // 关键字
    If, ElseIf, Else, Then, End, Loop,
    Var, Function, Have, Return,
    Continue, Break,
    And, Or, Not, Is, Default,
    Import,

    // 运算符
    Plus, Minus, Multiply, Divide, Modulo, Power,
    BitAnd, BitOr, BitXor, BitShiftLeft, BitShiftRight,
    Assign,   // =
    Less, LessEqual, Greater, GreaterEqual,

    //
    LeftParen, RightParen,  // ()
    Semicolon,   // ;
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

    // 特殊
    {TokenType::Eof, "Eof"}
};

class Lexer {
public:
    Lexer(const std::string &src, bool dbg_mode = false);
    ~Lexer();

    std::vector<Token> tokenize();
private:
    const std::string &src;
    std::vector<Token> tokens;
    int start;     // 0
    int current;
    int line;      // 1
    bool _dbg_mode;

    // 逐字扫描逻辑
    void scan();
    // 标识符判断
    void identifier();
    // 数字判断
    void number();
    // 字符串判断
    void string(char quote);

    // Utility
    char advance();    // 指针向前
    char peek() const;       // 查看当前
    char next_peek();
    bool match(char expected);
    bool is_end() const;
    void add_token(TokenType type);
    void error(const std::string &msg) const;

    void _dbg_print_tokens() const;

};


#endif //TOMORI_LANG_CPP_LEXER_H