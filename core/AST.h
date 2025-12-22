//
// Created by Alphazer01214 on 2025/12/21.
//

#ifndef TOMORI_LANG_CPP_AST_H
#define TOMORI_LANG_CPP_AST_H

#include "Lexer.h"
#include <vector>
#include <string>
#include <iostream>

// ==================== Expression Nodes ====================

struct Expression {
    virtual ~Expression() = default;
    virtual void print(int indent = 0) const = 0;

protected:
    static void pad(int indent) {
        std::cout << std::string(indent, ' ');
    }
};

struct LiteralExpression : public Expression {
    Token value;
    LiteralExpression(Token value) : value(value) {}

    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "Literal(" << value.token << ")\n";
    }
};

struct VariableExpression : public Expression {
    Token name;
    VariableExpression(Token name) : name(name) {}

    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "Variable(" << name.token << ")\n";
    }
};

struct UnaryExpression : public Expression {
    Token op;
    Expression* right;
    UnaryExpression(Token op, Expression* right) : op(op), right(right) {}

    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "Unary(" << op.token << ")\n";
        if (right) right->print(indent + 2);
    }
};

struct BinaryExpression : public Expression {
    Token op;
    Expression* left;
    Expression* right;
    BinaryExpression(Expression* left, Token op, Expression* right)
        : op(op), left(left), right(right) {}

    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "Binary(" << op.token << ")\n";
        if (left)  left->print(indent + 2);
        if (right) right->print(indent + 2);
    }
};

struct AssignExpression : public Expression {
    Token name;
    Expression* value;
    AssignExpression(Token name, Expression* value) : name(name), value(value) {}
    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "AssignExpression(" << name.token << ")\n";
        if (value) value->print(indent + 4);
    }
};

struct GroupingExpression : public Expression {
    Expression* expression;
    GroupingExpression(Expression* expression) : expression(expression) {}

    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "Group\n";
        if (expression) expression->print(indent + 2);
    }
};

// ==================== Statement Nodes ====================

struct Statement {
    virtual ~Statement() = default;
    virtual void print(int indent = 0) const = 0;

protected:
    static void pad(int indent) {
        std::cout << std::string(indent, ' ');
    }
};

struct ExpressionStatement : public Statement {
    Expression* expression;
    ExpressionStatement(Expression* expression) : expression(expression) {}

    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "ExpressionStatement\n";
        if (expression) expression->print(indent + 2);
    }
};

struct VariableStatement : public Statement {
    Token name;
    Expression* initializer;
    VariableStatement(Token name, Expression* initializer)
        : name(name), initializer(initializer) {}

    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "VariableStatement(" << name.token << ")\n";
        if (initializer) initializer->print(indent + 2);
    }
};

struct ReturnStatement : public Statement {
    Expression* value;
    ReturnStatement(Expression* expression) : value(expression) {}

    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "ReturnStatement\n";
        if (value) value->print(indent + 2);
    }
};

struct BreakStatement : public Statement {
    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "BreakStatement\n";
    }
};

struct ContinueStatement : public Statement {
    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "ContinueStatement\n";
    }
};

struct IfStatement : public Statement {
    Expression* condition;
    std::vector<Statement*> then_branches;
    std::vector<std::pair<Expression*, std::vector<Statement*>>> elseif_branches;
    std::vector<Statement*> else_branches;

    IfStatement(
        Expression* condition,
        std::vector<Statement*> thens,
        std::vector<std::pair<Expression*, std::vector<Statement*>>> elseifs,
        std::vector<Statement*> elses
    )
        : condition(condition),
          then_branches(std::move(thens)),
          elseif_branches(std::move(elseifs)),
          else_branches(std::move(elses)) {}

    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "IfStatement\n";

        pad(indent + 2);
        std::cout << "Condition:\n";
        if (condition) condition->print(indent + 4);

        pad(indent + 2);
        std::cout << "Then:\n";
        for (auto* stmt : then_branches) {
            stmt->print(indent + 4);
        }

        for (const auto& elseif : elseif_branches) {
            pad(indent + 2);
            std::cout << "ElseIf:\n";
            elseif.first->print(indent + 4);
            for (auto* stmt : elseif.second) {
                stmt->print(indent + 4);
            }
        }

        if (!else_branches.empty()) {
            pad(indent + 2);
            std::cout << "Else:\n";
            for (auto* stmt : else_branches) {
                stmt->print(indent + 4);
            }
        }
    }
};

struct LoopStatement : public Statement {
    Expression* condition;
    std::vector<Statement*> loop_body;

    LoopStatement(Expression* condition, std::vector<Statement*> loop_body)
        : condition(condition), loop_body(std::move(loop_body)) {}

    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "LoopStatement\n";

        pad(indent + 2);
        std::cout << "Condition:\n";
        if (condition) condition->print(indent + 4);

        pad(indent + 2);
        std::cout << "Body:\n";
        for (auto* stmt : loop_body) {
            stmt->print(indent + 4);
        }
    }
};

struct FunctionStatement : public Statement {
    Token name;
    std::vector<Token> parameters;
    std::vector<Statement*> body;
    FunctionStatement(Token name, std::vector<Token> parameters, std::vector<Statement*> body): name(name), parameters(parameters), body(std::move(body)) {}
    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "FunctionStatement\n";
        pad(indent + 2);
        std::cout << "Parameters:\n";
        for (auto parameter : parameters) {
            std::cout << "\t" << parameter.token << "\n";
        }
        pad(indent + 2);
        std::cout << "Body:\n";
        for (auto* stmt : body) {
            stmt->print(indent + 4);
        }
    }
};

#endif // TOMORI_LANG_CPP_AST_H
