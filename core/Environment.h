// 作用域，其实就是大括号里的内容
// Created by Alphazer01214 on 2025/12/22.
//

#ifndef TOMORI_LANG_CPP_ENVIRONMENT_H
#define TOMORI_LANG_CPP_ENVIRONMENT_H
#include "Define.h"
#include "Value.h"

class Environment {
public:
    Environment(): parent(nullptr){}
    Environment(Environment *parent): parent(parent) {};
    ~Environment(){
        for (auto &[_, val]: values) {
            delete val;
        }
    }
    void define(std::string name, Value *value);
    void assign(std::string name, Value *value);
    Value *get_value(std::string name);

private:
    Environment *parent;      // 指向上一级
    std::map<std::string, Value*> values;

    static void error(const std::string &msg);
    static void warning(const std::string &msg);
};


#endif //TOMORI_LANG_CPP_ENVIRONMENT_H