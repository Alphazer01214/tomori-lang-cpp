//
// Created by Alphazer01214 on 2025/12/22.
//

#ifndef TOMORI_LANG_CPP_INTERPRETER_H
#define TOMORI_LANG_CPP_INTERPRETER_H
#include "Parser.h"
#include "Environment.h"
#include "Builtin.h"


class Interpreter {
public:
    Interpreter() {
        current_env = new Environment();
        init_builtins();
    }
    ~Interpreter() = default;
    void interpret(const std::vector<Statement*>& statements);
    void init_builtins();

private:
    Environment *current_env;

    Value *execute_statements(const std::vector<Statement *> &statements, Environment *env);
    Value *execute_statement(Statement *st);
    Value *execute_expression(Expression *expr);       // 执行表达式 -> 计算得到Value

    void define_function(FunctionStatement *st);
    Value *define_variable(VariableStatement *st);
    Value *life_is_if_and_lie(IfStatement *if_st);
    Value *handle_loop(LoopStatement *loop_st);
    // Value *return_value(ReturnStatement *ret_st);     // 错误的，因为遇到return是信号，需要立即返回，因此需要nullptr
    Value *function_call(CallExpression *call_expr);
    Value *literal_value(LiteralExpression *expr);
    // Value *assign_variable(VariableExpression *var_expr);
    Value *unary_operation(UnaryExpression *unary_expr);
    Value *binary_operation(BinaryExpression *binary_expr);
    Value *assign_operation(AssignExpression *assign_expr);
    Value *add_operation(Expression *left, Expression *right);
    Value *sub_operation(Expression *left, Expression *right);
    Value *mul_operation(Expression *left, Expression *right);
    Value *div_operation(Expression *left, Expression *right);
    Value *pow_operation(Expression *left, Expression *right);
    Value *mod_operation(Expression *left, Expression *right);
    Value *is_operation(Expression *left, Expression *right);
    Value *isnot_operation(Expression *left, Expression *right);
    Value *greater_than(Expression *left, Expression *right);
    Value *greater_than_or_equal(Expression *left, Expression *right);
    Value *less_than(Expression *left, Expression *right);
    Value *less_than_or_equal(Expression *left, Expression *right);
    ValueType infer_type(Value *v);
    bool is_true(Value *y);

    void error(const std::string &msg);
    static void warning(const std::string &msg);
};


#endif //TOMORI_LANG_CPP_INTERPRETER_H