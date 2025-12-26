
//
// Created by Alphazer01214 on 2025/12/24.
//

#ifndef TOMORI_LANG_CPP_VALUE_H
#define TOMORI_LANG_CPP_VALUE_H

#include "AST.h"
#include "Define.h"
class Environment;
class Statement;
class Interpreter;

enum class ValueType {
    Integer,
    Float,
    String,
    Boolean,
    List,
    Function,
    BuiltinFunction,
    Break,
    Continue,
    Error,
    Mygo    // void
};

enum class ErrorType {

};

static std::unordered_map<ValueType, std::string> value_type_to_string = {
    {ValueType::Integer, "Integer"},
    {ValueType::Float, "Float"},
    {ValueType::String, "String"},
    {ValueType::Boolean, "Boolean"},
    {ValueType::Function, "Function"}
};

class Value {
public:
    ValueType type;
    Value(): type(ValueType::Mygo) {}
    virtual ~Value() = default;
    virtual std::string to_string() const = 0;
    std::string get_type_string() const {
        return value_type_to_string[type];
    }
};

class MygoValue : public Value {
public:
    MygoValue() {
        this->type = ValueType::Mygo;
    }
    std::string to_string() const override {
        return "<MyGO!!!!!>";
    }
};

class IntegerValue: public Value {
public:
    long long value;
    IntegerValue(long long v): value(v) {
        this->type = ValueType::Integer;
    }
    double to_float() const {
        return static_cast<double>(value);
    }
    std::string to_string() const override{
        return std::to_string(value);
    }
};

class FloatValue: public Value {
public:
    double value;
    FloatValue(double v): value(v) {
        this->type = ValueType::Float;
    }
    long long to_integer() const {
        return static_cast<long long>(value);
    }
    std::string to_string() const override {
        return std::to_string(value);
    }
};

class StringValue: public Value {
public:
    std::string value;
    StringValue(std::string v): value(std::move(v)) {
        this->type = ValueType::String;
    }
    std::string to_string() const override {
        return value;
    }
    long long to_integer() const {
        return std::stoll(value);
    }
    double to_float() const {
        return std::stod(value);
    }
};

class BooleanValue: public Value {
public:
    bool value;
    BooleanValue(bool v): value(v) {
        this->type = ValueType::Boolean;
    }
    long long to_integer() const {
        return value ? 1 : 0;
    }
    double to_float() const {
        return value ? 1.0 : 0.0;
    }
    std::string to_string() const override {
        return value ? "true" : "false";
    }
};

class ListValue: public Value {
public:
    std::vector<Value*> values;
    ListValue(std::vector<Value*> v): values(std::move(v)) {}
    std::string to_string() const override {
        std::string res;
        for (auto v: values) {
            res += v->to_string() + " ";
        }
        return "<list>" + res;
    }
};

class FunctionValue: public Value {
public:
    // 神秘的函数指针语法
    bool is_builtin;
    using builtin_fn = Value* (*)(Interpreter *, std::vector<Value*> &);
    builtin_fn fn = nullptr;
    std::vector<std::string> args;
    std::vector<Statement*> statements;
    Environment *env;

    FunctionValue(std::vector<std::string> _args, std::vector<Statement*> sts, Environment *env, bool built_in=false): args(_args), statements(std::move(sts)), env(env),  is_builtin(built_in){
        this->type = ValueType::Function;
    }

    explicit FunctionValue(builtin_fn func): fn(func) {
        this->type = ValueType::BuiltinFunction;
    }
    std::string to_string() const override {
        return is_builtin ? "<builtin function>" : "<function>";
    }
};

// class BuiltinFunctionValue: public Value {
// public:
//     // 接收参数列表
//     std::string function_name;
//     std::vector<Value*> values;
// };

class BreakValue: public Value {
public:
    BreakValue(){
        this->type = ValueType::Break;
    }
    std::string to_string() const override {
        return "<break>";
    }
};

class ContinueValue: public Value {
public:
    ContinueValue() {
        this->type = ValueType::Continue;
    }
    std::string to_string() const override {
        return "<continue>";
    }
};


#endif //TOMORI_LANG_CPP_VALUE_H