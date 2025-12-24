//
// Created by Alphazer01214 on 2025/12/22.
//

#include "Interpreter.h"

#include <functional>


Value *Interpreter::execute_statements(const std::vector<Statement *> &statements, Environment *env) {
    // 需要处理多个statement，说明进入新环境
    Environment *prev_env = current_env;
    current_env = env;
    for (auto stmt : statements) {
        // 执行函数体内 statements, 可能遇到return，返回对应Value
        Value *ret = execute_statement(stmt);
        if (ret) {
            // 接收到return信号，提前复原现场
            current_env = prev_env;
            return ret;
        }
    }
    // 复原现场
    current_env = prev_env;

    return nullptr;    // 没有return
}

Value *Interpreter::execute_statement(Statement *st) {
    if (auto *ret_st = dynamic_cast<ReturnStatement *>(st)) {
        return execute_expression(ret_st->value);
    }
    if (auto *fn_st = dynamic_cast<FunctionStatement *>(st)) {
        define_function(fn_st);
        // 只define，没有进入，因此按照 func...then...return end 结束
        return nullptr;
    }
    if (auto *var_st = dynamic_cast<VariableStatement *>(st)) {
        define_variable(var_st);
        return nullptr;
    }
    if (auto *exp_st = dynamic_cast<ExpressionStatement *>(st)) {
        execute_expression(exp_st->expression);
        return nullptr;
    }
    if (auto *if_st = dynamic_cast<IfStatement *>(st)) {

    }
    if (auto *loop_st = dynamic_cast<LoopStatement *>(st)) {

    }
    error("Unknown statement.");
    return nullptr;
}

Value *Interpreter::execute_expression(Expression *expr) {
    if (auto *call = dynamic_cast<CallExpression *>(expr)) {
        return function_call(call);
    }
    if (auto *var = dynamic_cast<VariableExpression *>(expr)) {
        // final
        return current_env->get_value(var->name.token);
    }
    if (auto *unary = dynamic_cast<UnaryExpression *>(expr)) {
        return unary_operation(unary);
    }
    if (auto *lit = dynamic_cast<LiteralExpression *>(expr)) {
        return literal_value(lit);
    }
    error("Unknown expression.");
    return nullptr;
}

void Interpreter::life_is_if_and_lie(IfStatement *st) {
    /*    Expression* condition;
    std::vector<Statement*> then_branches;
    std::vector<std::pair<Expression*, std::vector<Statement*>>> elseif_branches;
    std::vector<Statement*> else_branches;
    */
}

void Interpreter::define_function(FunctionStatement *st) {
    // parse vector<Token>
    std::string function_name = st->name.token;
    std::vector<std::string> args;
    for (auto &t: st->parameters) {
        args.push_back(t.token);
    }
    auto func = new FunctionValue(args, st->body, current_env);
    current_env->define(function_name, func);
}

void Interpreter::define_variable(VariableStatement *var_st) {
    std::string variable_name = var_st->name.token;
    Value *right = execute_expression(var_st->initializer);
    current_env->define(variable_name, right);
}

Value *Interpreter::function_call(CallExpression *call_expr) {
    // call_expr: *call_to(Identifier aka VariableExpression, have Token name) , args(several expressions)
    // FunctionValue: args: strings, sts: statements, env
    auto call_to = execute_expression(call_expr);    // -> function_value
    auto *fn = dynamic_cast<FunctionValue*>(call_to);
    if (!fn) {
        error("Attempt to call non-function");
        return nullptr;
    }
    int required_args = fn->args.size();
    int current_args = call_expr->args.size();

    if (required_args != current_args) {
        error("Called function requires " + std::to_string(required_args) + " arguments, but currently have " + std::to_string(current_args) + ".");
        return nullptr;
    }
    // call func env
    Environment *call_env = new Environment(fn->env);
    // bind args
    for (int i = 0; i < required_args; i++) {
        Value *arg = execute_expression(call_expr->args[i]);
        call_env->define(fn->args[i], arg);
    }

    // call 之后的结果是value
    return execute_statements(fn->statements, call_env);
}

// Value *Interpreter::return_value(ReturnStatement *ret_st) {
//     return execute_expression(ret_st->value);
// }

Value *Interpreter::literal_value(LiteralExpression *literal_expr) {
    // final
    TokenType type = literal_expr->value.type;
    std::string value = literal_expr->value.token;
    if (type == TokenType::Number) {
        const auto dot_pos = value.find('.');
        if (dot_pos == std::string::npos) {
            return new IntegerValue(std::stoll(value));
        } else {
            return new FloatValue(std::stod(value));
        }
    }

    if (type == TokenType::String) {
        return new StringValue(value);
    }

    if (type == TokenType::Boolean) {
        return new BooleanValue(value == "true");
    }
    error("Unknown error occurred when executing a literal expression.");

    return nullptr;
}

Value *Interpreter::unary_operation(UnaryExpression *unary_expr) {
    // op, right
    // negative not
    TokenType type = unary_expr->op.type;
    Value *right = execute_expression(unary_expr->right);
    if (type == TokenType::Not) {
        if (auto *int_v = dynamic_cast<IntegerValue *>(right)) {
            return new BooleanValue(!int_v->value);
        }
        if (auto *float_v = dynamic_cast<FloatValue *>(right)) {
            return new BooleanValue(!float_v->value);
        }
        if (auto *bool_v = dynamic_cast<BooleanValue *>(right)) {
            return new BooleanValue(!bool_v->value);
        }
        return new BooleanValue(false);
    }
    if (type == TokenType::Minus) {
        if (auto *int_v = dynamic_cast<IntegerValue *>(right)) {
            return new IntegerValue(-int_v->value);
        }
        if (auto *float_v = dynamic_cast<FloatValue *>(right)) {
            return new FloatValue(-float_v->value);
        }
        if (auto *bool_v = dynamic_cast<BooleanValue *>(right)) {
            return new BooleanValue(!bool_v->value);
        }
        if (auto *str_v = dynamic_cast<StringValue *>(right)) {
            std::string rev_str = str_v->value;
            std::reverse(rev_str.begin(), rev_str.end());
            return new StringValue(rev_str);
        }
        error("Cannot execute minus operation with type " + reverse_keywords[type]);
    }
    error("Unknown error occurred when executing an unary operation.");
    return nullptr;
}

Value *Interpreter::binary_operation(BinaryExpression *binary_expr) {
    TokenType type = binary_expr->op.type;
    // auto left = binary_expr->left;
    // auto right = binary_expr->right;
    if (type == TokenType::Plus) {
        return add_operation(binary_expr->left, binary_expr->right);
    }
    if (type == TokenType::Minus) {
        return sub_operation(binary_expr->left, binary_expr->right);
    }
    if (type == TokenType::Multiply) {
        return mul_operation(binary_expr->left, binary_expr->right);
    }
    if (type == TokenType::Divide) {
        return div_operation(binary_expr->left, binary_expr->right);
    }
    if (type == TokenType::Is) {
        return is_operation(binary_expr->left, binary_expr->right);
    }
    if (type == TokenType::IsNot) {
        return isnot_operation(binary_expr->left, binary_expr->right);
    }
    if (type == TokenType::Greater) {
        return greater_than(binary_expr->left, binary_expr->right);
    }
    if (type == TokenType::Less) {
        return less_than(binary_expr->left, binary_expr->right);
    }
    if (type == TokenType::GreaterEqual) {
        return greater_than_or_equal(binary_expr->left, binary_expr->right);
    }
    if (type == TokenType::LessEqual) {
        return less_than_or_equal(binary_expr->left, binary_expr->right);
    }
    error("Unknown error occurred when executing a binary operation.");
    return nullptr;
}

Value *Interpreter::assign_operation(AssignExpression *assign_expr) {
    std::string name = assign_expr->name.token;
    current_env->assign(name, execute_expression(assign_expr->value));
    return current_env->get_value(name);
}

Value* Interpreter::add_operation(Expression* left, Expression* right) {
    Value* l = execute_expression(left);
    Value* r = execute_expression(right);

    // int + int
    if (auto* li = dynamic_cast<IntegerValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            return new IntegerValue(li->value + ri->value);
        }
        if (auto* rf = dynamic_cast<FloatValue*>(r)) {
            return new FloatValue(li->value + rf->value);
        }
    }

    // float + int / float
    if (auto* lf = dynamic_cast<FloatValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            return new FloatValue(lf->value + ri->value);
        }
        if (auto* rf = dynamic_cast<FloatValue*>(r)) {
            return new FloatValue(lf->value + rf->value);
        }
    }

    // string + string
    if (auto* ls = dynamic_cast<StringValue*>(l)) {
        if (auto* rs = dynamic_cast<StringValue*>(r)) {
            return new StringValue(ls->value + rs->value);
        }
    }

    error("Invalid operands for +");
    return new MygoValue();
}

Value* Interpreter::sub_operation(Expression* left, Expression* right) {
    Value* l = execute_expression(left);
    Value* r = execute_expression(right);

    // int - int / float
    if (auto* li = dynamic_cast<IntegerValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            return new IntegerValue(li->value - ri->value);
        }
        if (auto* rf = dynamic_cast<FloatValue*>(r)) {
            return new FloatValue(li->value - rf->value);
        }
    }

    // float - int / float
    if (auto* lf = dynamic_cast<FloatValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            return new FloatValue(lf->value - ri->value);
        }
        if (auto* rf = dynamic_cast<FloatValue*>(r)) {
            return new FloatValue(lf->value - rf->value);
        }
    }

    error("Invalid operands for -");
    return new MygoValue();
}

Value* Interpreter::mul_operation(Expression* left, Expression* right) {
    Value* l = execute_expression(left);
    Value* r = execute_expression(right);

    if (auto* li = dynamic_cast<IntegerValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            return new IntegerValue(li->value * ri->value);
        }
        if (auto* rf = dynamic_cast<FloatValue*>(r)) {
            return new FloatValue(li->value * rf->value);
        }
    }

    if (auto* lf = dynamic_cast<FloatValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            return new FloatValue(lf->value * ri->value);
        }
        if (auto* rf = dynamic_cast<FloatValue*>(r)) {
            return new FloatValue(lf->value * rf->value);
        }
    }
    // str * 114514
    if (auto* ls = dynamic_cast<StringValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            std::string result;
            for (int i = 0; i < ri->value; i++) {
                result += ls->value;
            }
            return new StringValue(result);
        }
    }
    error("Invalid operands for *");
    return new MygoValue();
}

Value* Interpreter::div_operation(Expression* left, Expression* right) {
    Value* l = execute_expression(left);
    Value* r = execute_expression(right);

    // int / int
    if (auto* li = dynamic_cast<IntegerValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            if (ri->value == 0) {
                error("Division by zero");
                return new MygoValue();
            }
            return new FloatValue(
                static_cast<double>(li->value) / ri->value
            );
        }
        if (auto* rf = dynamic_cast<FloatValue*>(r)) {
            if (rf->value == 0.0) {
                error("Division by zero");
                return new MygoValue();
            }
            return new FloatValue(li->value / rf->value);
        }
    }

    // float / int / float
    if (auto* lf = dynamic_cast<FloatValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            if (ri->value == 0) {
                error("Division by zero");
                return new MygoValue();
            }
            return new FloatValue(lf->value / ri->value);
        }
        if (auto* rf = dynamic_cast<FloatValue*>(r)) {
            if (rf->value == 0.0) {
                error("Division by zero");
                return new MygoValue();
            }
            return new FloatValue(lf->value / rf->value);
        }
    }

    error("Invalid operands for /");
    return new MygoValue();
}

Value* Interpreter::is_operation(Expression* left, Expression* right) {
    Value* l = execute_expression(left);
    Value* r = execute_expression(right);

    // int == int
    if (auto* li = dynamic_cast<IntegerValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            return new BooleanValue(li->value == ri->value);
        }
        if (auto* rf = dynamic_cast<FloatValue*>(r)) {
            return new BooleanValue(li->value == rf->value);
        }
    }

    // float == int / float
    if (auto* lf = dynamic_cast<FloatValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            return new BooleanValue(lf->value == ri->value);
        }
        if (auto* rf = dynamic_cast<FloatValue*>(r)) {
            return new BooleanValue(lf->value == rf->value);
        }
    }

    // bool == bool
    if (auto* lb = dynamic_cast<BooleanValue*>(l)) {
        if (auto* rb = dynamic_cast<BooleanValue*>(r)) {
            return new BooleanValue(lb->value == rb->value);
        }
    }

    // string == string
    if (auto* ls = dynamic_cast<StringValue*>(l)) {
        if (auto* rs = dynamic_cast<StringValue*>(r)) {
            return new BooleanValue(ls->value == rs->value);
        }
    }

    return new BooleanValue(false);
}

Value* Interpreter::isnot_operation(Expression* left, Expression* right) {
    auto* eq = dynamic_cast<BooleanValue*>(
        is_operation(left, right)
    );
    return new BooleanValue(!eq->value);
}

Value* Interpreter::greater_than(Expression* left, Expression* right) {
    Value* l = execute_expression(left);
    Value* r = execute_expression(right);

    if (auto* li = dynamic_cast<IntegerValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            return new BooleanValue(li->value > ri->value);
        }
        if (auto* rf = dynamic_cast<FloatValue*>(r)) {
            return new BooleanValue(li->value > rf->value);
        }
    }

    if (auto* lf = dynamic_cast<FloatValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            return new BooleanValue(lf->value > ri->value);
        }
        if (auto* rf = dynamic_cast<FloatValue*>(r)) {
            return new BooleanValue(lf->value > rf->value);
        }
    }

    error("Invalid operands for >");
    return new BooleanValue(false);
}

Value* Interpreter::greater_than_or_equal(Expression* left, Expression* right) {
    Value* l = execute_expression(left);
    Value* r = execute_expression(right);

    if (auto* li = dynamic_cast<IntegerValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            return new BooleanValue(li->value >= ri->value);
        }
        if (auto* rf = dynamic_cast<FloatValue*>(r)) {
            return new BooleanValue(li->value >= rf->value);
        }
    }

    if (auto* lf = dynamic_cast<FloatValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            return new BooleanValue(lf->value >= ri->value);
        }
        if (auto* rf = dynamic_cast<FloatValue*>(r)) {
            return new BooleanValue(lf->value >= rf->value);
        }
    }

    error("Invalid operands for >=");
    return new BooleanValue(false);
}

Value* Interpreter::less_than(Expression* left, Expression* right) {
    Value* l = execute_expression(left);
    Value* r = execute_expression(right);

    if (auto* li = dynamic_cast<IntegerValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            return new BooleanValue(li->value < ri->value);
        }
        if (auto* rf = dynamic_cast<FloatValue*>(r)) {
            return new BooleanValue(li->value < rf->value);
        }
    }

    if (auto* lf = dynamic_cast<FloatValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            return new BooleanValue(lf->value < ri->value);
        }
        if (auto* rf = dynamic_cast<FloatValue*>(r)) {
            return new BooleanValue(lf->value < rf->value);
        }
    }

    error("Invalid operands for <");
    return new BooleanValue(false);
}

Value* Interpreter::less_than_or_equal(Expression* left, Expression* right) {
    Value* l = execute_expression(left);
    Value* r = execute_expression(right);

    if (auto* li = dynamic_cast<IntegerValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            return new BooleanValue(li->value <= ri->value);
        }
        if (auto* rf = dynamic_cast<FloatValue*>(r)) {
            return new BooleanValue(li->value <= rf->value);
        }
    }

    if (auto* lf = dynamic_cast<FloatValue*>(l)) {
        if (auto* ri = dynamic_cast<IntegerValue*>(r)) {
            return new BooleanValue(lf->value <= ri->value);
        }
        if (auto* rf = dynamic_cast<FloatValue*>(r)) {
            return new BooleanValue(lf->value <= rf->value);
        }
    }

    error("Invalid operands for <=");
    return new BooleanValue(false);
}

ValueType Interpreter::infer_type(Value *v) {
    auto *int_v = dynamic_cast<IntegerValue *>(v);
    auto *float_v = dynamic_cast<FloatValue *>(v);
    auto *bool_v = dynamic_cast<BooleanValue *>(v);
    auto *str_v = dynamic_cast<StringValue *>(v);
    if (int_v) {
        return ValueType::Integer;
    }
    if (float_v) {
        return ValueType::Float;
    }
    if (bool_v) {
        return ValueType::Boolean;
    }
    if (str_v) {
        return ValueType::String;
    }
    return ValueType::Mygo;
}

bool Interpreter::is_true(Value *v) {
    if (!v) return false;

    if (auto* b = dynamic_cast<BooleanValue*>(v)) {
        return b->value;
    }
    if (auto* i = dynamic_cast<IntegerValue*>(v)) {
        return i->value != 0;
    }
    if (auto* f = dynamic_cast<FloatValue*>(v)) {
        return f->value != 0.0;
    }
    if (auto* s = dynamic_cast<StringValue*>(v)) {
        return !s->value.empty();
    }
    return false;
}

void Interpreter::error(const std::string &msg) {
    std::cerr<<"[Runtime Error] "<<msg<<std::endl;
}
void Interpreter::warning(const std::string &msg) {
    std::cerr<<"[Warning] "<<msg<<std::endl;
}