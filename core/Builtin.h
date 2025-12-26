//
// Created by Alphazer01214 on 2025/12/26.
//

#ifndef TOMORI_LANG_CPP_BUILTIN_H
#define TOMORI_LANG_CPP_BUILTIN_H
#include "Value.h"

// util func
inline bool _is_all_value_integer(const std::vector<Value*> &values) {
    for (auto v: values) {
        if (v->type == ValueType::Integer) {
            continue;
        }
        return false;
    }
    return true;
}

inline bool _is_have_float(const std::vector<Value*> &values) {
    for (auto v: values) {
        if (v->type == ValueType::Float) {
            return true;
        }
    }
    return false;
}

inline bool _is_all_value_string(const std::vector<Value*> &values) {
    for (auto v: values) {
        if (v->type == ValueType::String) {
            continue;
        }
        return false;
    }
    return true;
}

// builtins
inline Value* builtin_print(Interpreter* interp, std::vector<Value*>& args) {
    for (auto* v : args) {
        std::cout << v->to_string() << " ";
    }
    std::cout << std::endl;
    return new MygoValue(); // 相当于 void / nil
}

inline Value* builtin_sort(Interpreter* interp, std::vector<Value*>& args) {
    // 改进排序逻辑：如果是数字则按数值排，否则按字符串排
    std::sort(args.begin(), args.end(), [](const Value* a, const Value* b) {
        if (a->type == ValueType::Integer && b->type == ValueType::Integer) {
            return dynamic_cast<const IntegerValue*>(a)->value < dynamic_cast<const IntegerValue*>(b)->value;
        }
        return a->to_string() < b->to_string();
    });

    // 打印当前结果（调试用）
    for (size_t i = 0; i < args.size(); ++i) {
        std::cout << args[i]->to_string() << (i == args.size() - 1 ? "" : " ");
    }
    std::cout << std::endl;
    return new MygoValue();
    // return new ListValue(args);
}

inline Value* builtin_sort_dec(Interpreter *interp, std::vector<Value*>& args) {
    std::sort(args.begin(), args.end(), [](const Value* a, const Value* b) {
        if (a->type == ValueType::Integer && b->type == ValueType::Integer) {
            return dynamic_cast<const IntegerValue*>(a)->value < dynamic_cast<const IntegerValue*>(b)->value;
        }
        return a->to_string() < b->to_string();
    });

    // 打印当前结果（调试用）
    for (size_t i = 0; i < args.size(); ++i) {
        std::cout << args[i]->to_string() << (i == args.size() - 1 ? "" : " ");
    }
    std::cout << std::endl;
    return new MygoValue();
}
#endif //TOMORI_LANG_CPP_BUILTIN_H