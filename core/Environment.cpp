//
// Created by Alphazer01214 on 2025/12/22.
//

#include "Environment.h"


void Environment::define(std::string name, Value *value) {
    // Create a new variable
    if (values.contains(name)) {
        warning("Variable '" + name + "'<" + values[name]->get_type_string() + "> already defined, now redefined as <" + value->get_type_string() + ">.");
    }
    values[name] = value;
}

void Environment::assign(std::string name, Value *value) {
    if (values.contains(name)) {
        // 如果variable是定义在当前层
        ValueType ori_type = values[name]->type;
        ValueType tgt_type = value->type;
        if (ori_type == ValueType::BuiltinFunction || ori_type == ValueType::Function || tgt_type == ValueType::BuiltinFunction || tgt_type == ValueType::Function) {
            error("Assign a function is invalid.");
            return;
        }
        if (ori_type != tgt_type) {
            warning("Attempt to cast '" + name + "'<" + value_type_to_string[ori_type] + "> to " + "<" + value_type_to_string[tgt_type] + ">");
        }
        values[name] = value;
        return;
    }
    if (this->parent) {
        // variable 不在当前层，找 parent
        this->parent->assign(name, value);
        return;
    }
    error(name + " undefined");
}

Value *Environment::get_value(std::string name) {
    if (values.contains(name)) {
        // std::cout<<values[name]->to_string()<<std::endl;
        return values[name];
    }
    if (this->parent) {
        return this->parent->get_value(name);
    }
    error("Variable " + name + " undefined");
    return new MygoValue();
}

void Environment::clear() {
    for (const auto& [_, value]: values) {
        delete value;
    }
    values.clear();
}

void Environment::reset_value(const std::string& name) {
    if (values.contains(name)) {
        delete values[name];
    }
}

void Environment::error(const std::string &msg) {
    std::cerr<<"[Runtime Error] "<<msg<<std::endl;
}
void Environment::warning(const std::string &msg) {
    std::cerr<<"[Warning] "<<msg<<std::endl;
}
