//
// Created by Alphazer01214 on 2025/12/21.
//

#ifndef TOMORI_LANG_CPP_PARSER_H
#define TOMORI_LANG_CPP_PARSER_H
#include "Define.h"
#include "AST.h"


class Parser {
public:
    Parser(const std::vector<Token> &tokens, bool dbg_mode = false): tokens(tokens), current(0), _dbg_mode(dbg_mode), _error(false) {};
    ~Parser();
    Expression *parse_expression();
    Statement *parse_statement();
    std::vector<Statement*> parse();
    std::vector<Statement *> parse(const std::vector<Token> &_tokens);

private:
    std::vector<Token> tokens;
    int current;
    bool _dbg_mode;
    bool _error;

    // Parse Expression
    Expression *parsed_expressions;
    Expression *expr();
    Expression *assign_expr();   // x = true or false -> x = true
    Expression *or_expr();
    Expression *and_expr();
    Expression *not_expr();
    Expression *comp_expr();
    Expression *term_expr();     // + -
    Expression *factor_expr();   // * /
    Expression *power_expr();    // **
    Expression *unary_expr();    // negative, not
    Expression *primary_expr();
    Expression *call_expr();     // func + (
    Expression *finish_call_expr(Expression *call_to);

    // Parse Statements
    std::vector<Statement *> parsed_statements;
    Statement *st();
    Statement *var_st();       // 变量声明
    Statement *func_st();
    Statement *return_st();
    Statement *if_st();
    Statement *loop_st();
    Statement *expr_st();
    std::vector<Statement*> block(std::initializer_list<TokenType> terminators);

    Token advance();
    Token peek();     // current token
    Token previous(); // previous token
    TokenType current_type();
    void synchronize();
    bool match(TokenType type);
    bool check(TokenType type);
    Token consume(TokenType type, const std::string &msg);
    bool is_end() const;
    void clear();
    void error(const std::string &msg);
    static void message(const std::string &msg);
};


#endif //TOMORI_LANG_CPP_PARSER_H