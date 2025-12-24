//
// Created by Alphazer01214 on 2025/12/20.
//

#include "Lexer.h"

Lexer::Lexer(const std::string &src, bool dbg_mode): start(0), current(0), line(1), src(src), _dbg_mode(dbg_mode) {

}

Lexer::~Lexer() = default;

std::vector<Token> Lexer::tokenize() {
    while (!is_end()) {
        start = current;
        scan();
    }
    tokens.push_back({TokenType::Eof, "", line});
    if (_dbg_mode) {
        _dbg_print_tokens();
    }
    return tokens;
}

void Lexer::scan() {
    char ch = advance();   // read and step one
    switch (ch) {
        case ' ':
        case '\t':
        case '\r':
            break;
        case '\n':
            line++;
            break;
        case '/':
            // current ++
            if (match('/')) {
                while (peek() != '\n' && !is_end()) {
                    advance();
                }
            }else {
                add_token(TokenType::Divide);
            }
            break;
            // 运算符
        case '+':
            add_token(TokenType::Plus);
            break;
        case '-':
            add_token(TokenType::Minus);
            break;

        case '*':
            if (match('*')) {
                add_token(TokenType::Power);
            }
            else {
                add_token(TokenType::Multiply);
            }
            break;

        case '^':
            add_token(TokenType::BitXor);
            break;
        case '&':
            add_token(TokenType::BitAnd);
            break;
        case '|':
            add_token(TokenType::BitOr);
            break;

        case '=':
            add_token(TokenType::Assign);
            break;
        case '<':
            if (match('=')) {
                add_token(TokenType::LessEqual);
            } else {
                add_token(TokenType::Less);
            }
            break;
        case '>':
            if (match('=')) {
                add_token(TokenType::GreaterEqual);
            } else {
                add_token(TokenType::Greater);
            }
            break;
            // 符号
        case '(' :
            add_token(TokenType::LeftParen);
            break;
        case ')' :
            add_token(TokenType::RightParen);
            break;
        case ';' :
            add_token(TokenType::Semicolon);
            break;
        case ',':
            add_token(TokenType::Comma);
            break;
            // 字符串
        case '\'':
        case '\"':
            string(ch);
            break;

        default:
            if (isdigit(ch)) {
                number();
            } else if (isalpha(ch)) {
                identifier();
            } else {
                error("Unexpected character");
            }
    }
}

char Lexer::advance() {
    return src[current++];
}

char Lexer::peek() const {
    if (is_end()) {
        return '\0';
    }
    return src[current];
}

char Lexer::next_peek() {
    if (current + 1 >= src.length()) {
        return '\0';
    }
    return src[current+1];
}

bool Lexer::is_end() const {
    return current >= src.length();
}

void Lexer::add_token(const TokenType type) {
    const std::string token = src.substr(start, current - start);
    tokens.push_back({type, token, line});
}

void Lexer::identifier() {
    while (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_') {
        advance();
    }

    const std::string token = src.substr(start, current - start);

    auto it = keywords.find(token);
    if (it != keywords.end()) {
        add_token(it->second);
    } else {
        add_token(TokenType::Identifier);
    }
}

void Lexer::string(const char quote) {
    while (!is_end() && peek() != quote) {
        if (peek() == '\n') {
            line++;
        }
        advance();
    }
    if (is_end()) {
        error("Unterminated string");
        return;
    }
    advance();    // 跳过' "
    const std::string token = src.substr(start+1, current - start - 2);
    tokens.push_back({TokenType::String, token, line});
}

void Lexer::number() {
    while (std::isdigit(peek())) {
        advance();
    }

    if (peek() == '.' && std::isdigit(next_peek())) {
        advance();
        while (std::isdigit(peek())) {
            advance();
        }
    }
    add_token(TokenType::Number);
}

bool Lexer::match(char expected) {
    if (is_end()) return false;
    if (src[current] != expected) return false;
    current++;    // 注意吃字符
    return true;
}


void Lexer::error(const std::string &msg) const {
    std::cerr<<"[Lexer Error] line "<<line<<" message: "<<msg<<std::endl;
}

void Lexer::_dbg_print_tokens() const {
    for (const auto& token : tokens) {
        std::cout<<std::setw(20)<<("[" + reverse_keywords[token.type] + "] ")<<std::setw(20)<<token.token<<std::setw(10)<<" line "<<token.line<<std::endl;
    }
}
