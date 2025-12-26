//
// Created by Alphazer01214 on 2025/12/20.
//

#ifndef TOMORI_LANG_CPP_LEXER_H
#define TOMORI_LANG_CPP_LEXER_H
#include "Define.h"

class Lexer {
public:
    Lexer(bool dbg_mode = false): _dbg_mode(dbg_mode), start(0), current(0), line(1), _notify_error(false) {};
    ~Lexer();

    std::vector<Token> tokenize();
    std::vector<Token> tokenize(const std::string &src, bool notify = true, bool need_dbg = false);
private:
    std::string _src;
    std::vector<Token> tokens;
    bool _notify_error;
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
    void error(const std::string &msg);
    void terminate();

    void _dbg_print_tokens() const;

};


#endif //TOMORI_LANG_CPP_LEXER_H