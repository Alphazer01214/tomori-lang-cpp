//
// Created by Alphazer01214 on 2025/12/21.
//

#include "Parser.h"

Parser::~Parser() = default;

/*
 *  1. ()
    2. **
    3. * /
    4. + -
    5. < > is isnot
    6. not
    7. and
    8. or
*/

std::vector<Statement *> Parser::parse() {
    std::vector<Statement *> res;
    while (!is_end()) {
        res.emplace_back(st());
    }
    return res;
}

Statement *Parser::st() {
    if (match(TokenType::Var)) {
        return var_st();
    }
    if (match(TokenType::If)) {
        return if_st();
    }
    if (match(TokenType::Loop)) {
        return loop_st();
    }
    if (match(TokenType::Function)) {
        // 函数定义阶段
        return func_st();
    }
    if (match(TokenType::Return)) {
        return return_st();
    }
    if (match(TokenType::Break)) {
        return new BreakStatement();
    }
    if (match(TokenType::Continue)) {
        return new ContinueStatement();
    }
    return expr_st();
}

Statement *Parser::var_st() {
    Token name = consume(TokenType::Identifier, "Expect a variable name.");
    Expression *initializer = nullptr;
    if (match(TokenType::Assign)) {
        initializer = expr();
    }
    return new VariableStatement(name, initializer);
}

Statement *Parser::if_st() {
    // if (exp) then ... elseif (exp) then ... else ... end
    Expression *condition = expr();
    std::vector<Statement *> then_block;
    std::vector<std::pair<Expression*, std::vector<Statement*>>> elif_blocks;
    std::vector<Statement *> else_block;
    consume(TokenType::Then, "Expect 'then' after 'if'.");
    
    // then
    while (current_type() != TokenType::Else && current_type() != TokenType::ElseIf && current_type() != TokenType::End && !is_end()) {
        // std::cout << "then"<<std::endl;
        then_block.emplace_back(st());
    }

    // elseif
    while (match(TokenType::ElseIf)) {
        // std::cout << "elseif"<<std::endl;
        Expression *elif_cond = expr();
        std::vector<Statement*> elif_block;
        consume(TokenType::Then, "Expect 'then' after 'elseif'.");
        while (current_type() != TokenType::Else && current_type() != TokenType::ElseIf && current_type() != TokenType::End && !is_end()) {
            elif_block.emplace_back(st());
        }
        elif_blocks.emplace_back(elif_cond, elif_block);
    }

    // else
    if (match(TokenType::Else)) {
        while (current_type() != TokenType::End && !is_end()) {
            // std::cout << "else"<<std::endl;
            else_block.emplace_back(st());
        }
    }

    // end
    consume(TokenType::End, "If block not closed with 'end'.");
    return new IfStatement(condition, then_block, elif_blocks, else_block);
}

Statement *Parser::loop_st() {
    Expression *condition = nullptr;
    std::vector<Statement *> loop_block;

    // loop then
    if (check(TokenType::Then)) {
        advance(); // consume then
    } else {
        // loop cond then
        condition = expr();
        consume(TokenType::Then, "Expect 'then' after loop condition.");
    }

    while (current_type() != TokenType::End && !is_end()) {
        loop_block.emplace_back(st());
    }

    consume(TokenType::End, "Loop block not closed with 'end'.");
    return new LoopStatement(condition, loop_block);
}


Statement *Parser::func_st() {
    Token name = consume(TokenType::Identifier, "Expect a function name.");
    std::vector<Token> params;
    std::vector<Statement *> func_block;

    // have params
    if (current_type() == TokenType::Have) {
        advance();
        while (current_type() != TokenType::Then && !is_end()) {
            params.push_back(advance());
        }
    }
    // then block
    consume(TokenType::Then, "Expect 'then' after function.");

    while (current_type() != TokenType::End && !is_end()) {
        func_block.emplace_back(st());
    }
    consume(TokenType::End, "Function block not closed with 'end'.");
    return new FunctionStatement(name, params, func_block);
}

Statement* Parser::return_st() {
    Expression* value = nullptr;
    if (!check(TokenType::End) && !check(TokenType::Then) && !check(TokenType::Eof)) {
        value = expr();  // 解析返回值
    }
    return new ReturnStatement(value);
}


Statement *Parser::expr_st() {
    return new ExpressionStatement(expr());
}

// std::vector<Statement*> Parser::block(std::initializer_list<TokenType> terminators) {
//     std::vector<Statement*> res;
//     while (!is_end()) {
//         for (auto t : terminators) {
//             if (current_type() == t)
//                 return res;
//         }
//         res.emplace_back(st());
//     }
//     return res;
// }

Statement *Parser::parse_statement() {
    return st();
}

Expression *Parser::parse_expression() {
    if (_dbg_mode) {
        Expression *res = expr();
        if (res == nullptr) {
            return nullptr;
        }
        res->print();
        return res;
    }
    return expr();
}

Expression *Parser::expr() {
    return assign_expr();
}

Expression *Parser::assign_expr() {
    // 右结合的二元运算，需要递归，例如 a = b = c is a = (b = c)
    // 结合左值，就调用下一优先级
    Expression *left = or_expr();
    while (match(TokenType::Assign)) {
        Token op = previous();
        // 结合右值，就递归
        Expression *right = assign_expr();
        if (auto var = dynamic_cast<VariableExpression *>(left)) {
            // 如果左值是变量 var ，就赋值
            return new AssignExpression(var->name, right);
        } else {
            error("Wrong assignment expression.");
            return nullptr;
        }
    }
    return left;
}

Expression *Parser::or_expr() {
    Expression *res = and_expr();     // 解析priority更高的and，当作结果
    while (match(TokenType::Or)) {
        // 是 or， 吞掉token（此时为or operator）， 顺便递归解析右边的and
        Token op = previous();
        Expression *right = and_expr();
        res = new BinaryExpression(res, op, right);
    }
    return res;
}

Expression *Parser::and_expr() {
    Expression *res = not_expr();
    while (match(TokenType::And)) {
        Token op = previous();
        Expression *right = not_expr();
        res = new BinaryExpression(res, op, right);
    }
    return res;
}

Expression *Parser::not_expr() {
    // 右结合的一元运算
    if (match(TokenType::Not)) {
        Token op = previous();
        Expression *right = not_expr();
        return new UnaryExpression(op, right);
    }
    return comp_expr();
}

Expression *Parser::comp_expr() {
    Expression *res = term_expr();
    while (match(TokenType::Greater) || match(TokenType::GreaterEqual) || match(TokenType::Less) || match(TokenType::LessEqual) || match(TokenType::Is) || match(TokenType::IsNot)) {
        Token op = previous();
        Expression *right = term_expr();
        res = new BinaryExpression(res, op, right);
    }
    return res;
}

Expression *Parser::term_expr() {
    Expression *res = factor_expr();
    while (match(TokenType::Plus) || match(TokenType::Minus)) {
        Token op = previous();
        Expression *right = factor_expr();
        res = new BinaryExpression(res, op, right);
    }
    return res;
}

Expression *Parser::factor_expr() {
    Expression *res = power_expr();
    while (match(TokenType::Multiply) || match(TokenType::Divide)) {
        Token op = previous();
        Expression *right = power_expr();
        res = new BinaryExpression(res, op, right);
    }
    return res;
}

Expression *Parser::power_expr() {
    // 右结合
    Expression *res = unary_expr();
    while (match(TokenType::Power)) {
        Token op = previous();
        Expression *right = power_expr();
        res = new BinaryExpression(res, op, right);
    }
    return res;
}

Expression *Parser::unary_expr() {
    // 单右结合一元运算
    if (match(TokenType::Minus) || match(TokenType::Not)) {
        Token op = previous();
        Expression *right = unary_expr();
        return new UnaryExpression(op, right);
    }
    return call_expr();
}


Expression *Parser::call_expr() {
    Expression *res = primary_expr();
    if (match(TokenType::LeftParen)) {
        // 匹配到(，开始函数解析
        res = finish_call_expr(res);
    }
    return res;
}

Expression *Parser::primary_expr() {
    // if literal (variable value)
    if (match(TokenType::Number) || match(TokenType::String) || match(TokenType::Boolean)) {
        return new LiteralExpression(previous());
    }
    // if identifier
    if (match(TokenType::Identifier)) {
        return new VariableExpression(previous());
    }
    // if ()
    if (match(TokenType::LeftParen)) {
        Expression *res = expr();
        // 如果expr递归到最后都没找到 rightparen，就报错
        consume(TokenType::RightParen, "Expect ')' after expression.");
        return new GroupingExpression(res);
    }
    message("End of expression at " + reverse_keywords[current_type()] + ".");
    return nullptr;
}

Expression *Parser::finish_call_expr(Expression *call_to) {
    std::vector<Expression*> args;
    if (!check(TokenType::RightParen)) {
        // left ... right 中间有别的东西，说明有args: func(a, b, c, ...)
        do {
            args.push_back(expr());    // 解析
        }while (match(TokenType::Comma));
    }
    consume(TokenType::RightParen, "Expect ')' after function call.");
    return new CallExpression(call_to, args);
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        // current 匹配到，吃掉这个token，步进1
        advance();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) {
    return type == current_type();
}

Token Parser::advance() {
    return tokens[current++];
}

Token Parser::previous() {
    return tokens[current - 1];
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::consume(TokenType type, const std::string &msg) {
    if (check(type)) {
        return advance();
    }
    error(msg);
    return {};
}

TokenType Parser::current_type() {
    return peek().type;
}

bool Parser::is_end() const {
    return tokens[current].type == TokenType::Eof;
}

void Parser::error(const std::string &msg) {
    std::cerr<<"[Parser Error] " << msg << std::endl;
}

void Parser::message(const std::string &msg) {
    std::cerr<<"[Parser Message] "<<msg<<std::endl;
}
