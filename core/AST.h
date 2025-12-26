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
enum class ExpressionType {

};

class Expression {
public:
    virtual ~Expression() = default;
    virtual void print(int indent = 0) const = 0;

protected:
    static void pad(int indent) {
        std::cout << std::string(indent, ' ');
    }
};

class LiteralExpression : public Expression {
public:
    Token value;

    explicit LiteralExpression(Token value) : value(value) {}

    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "Literal(" << value.token << ")\n";
    }
};

class VariableExpression : public Expression {
public:
    Token name;

    explicit VariableExpression(Token name) : name(name) {}

    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "Variable(" << name.token << ")\n";
    }
};

class UnaryExpression : public Expression {
public:
    Token op;
    Expression* right;

    UnaryExpression(Token op, Expression* right)
        : op(op), right(right) {}

    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "Unary(" << op.token << ")\n";
        if (right) right->print(indent + 2);
    }
};

class BinaryExpression : public Expression {
public:
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

class AssignExpression : public Expression {
public:
    Token name;
    Expression* value;

    AssignExpression(Token name, Expression* value)
        : name(name), value(value) {}

    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "AssignExpression(" << name.token << ")\n";
        if (value) value->print(indent + 4);
    }
};

class GroupingExpression : public Expression {
public:
    Expression* expression;

    explicit GroupingExpression(Expression* expression)
        : expression(expression) {}

    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "Group\n";
        if (expression) expression->print(indent + 2);
    }
};

class CallExpression : public Expression {
public:
    Expression *call_to;
    std::vector<Expression*> args;
    explicit CallExpression(Expression* ct, std::vector<Expression*> ag): call_to(ct), args(ag) {}
    void print(int indent = 0) const override {
        pad(indent);
        std::cout<<"CallExpression"<<std::endl;
        call_to->print(indent + 2);
        pad(indent + 2);
        std::cout<<"Arguments:"<<std::endl;
        for (auto ag: args) {
            ag->print(indent + 4);
        }
    }
};

// class MygoExpression: public Expression {
//
// };

// ==================== Statement Nodes ====================

class Statement {
public:
    virtual ~Statement() = default;
    virtual void print(int indent = 0) const = 0;

protected:
    static void pad(int indent) {
        std::cout << std::string(indent, ' ');
    }
};

class ExpressionStatement : public Statement {
public:
    Expression* expression;

    explicit ExpressionStatement(Expression* expression)
        : expression(expression) {}

    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "ExpressionStatement\n";
        if (expression) expression->print(indent + 2);
    }
};

class VariableStatement : public Statement {
public:
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

class ReturnStatement : public Statement {
public:
    Expression* value;

    explicit ReturnStatement(Expression* expression)
        : value(expression) {}

    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "ReturnStatement\n";
        if (value) value->print(indent + 2);
    }
};

class BreakStatement : public Statement {
public:
    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "BreakStatement\n";
    }
};

class ContinueStatement : public Statement {
public:
    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "ContinueStatement\n";
    }
};

class IfStatement : public Statement {
public:
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

class LoopStatement : public Statement {
public:
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

class FunctionStatement : public Statement {
public:
    Token name;
    std::vector<Token> parameters;
    std::vector<Statement*> body;

    FunctionStatement(
        Token name,
        std::vector<Token> parameters,
        std::vector<Statement*> body
    )
        : name(name),
          parameters(std::move(parameters)),
          body(std::move(body)) {}

    void print(int indent = 0) const override {
        pad(indent);
        std::cout << "FunctionStatement\n";

        pad(indent + 2);
        std::cout<<"Name: \n";
        std::cout<<"\t"<< name.token << "\n";
        pad(indent + 2);
        std::cout << "Parameters:\n";
        for (const auto& parameter : parameters) {
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
