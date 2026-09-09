#pragma once
#include <unordered_map>
#include <string>

#include "Token.h"


class Environment {
    private:
        std::unordered_map<std::string, Literal> values;
    public:
        void define(const std::string& key, Literal value);
        Literal get(const Token& name);
       void assign(const Token& name, Literal value);
};