#pragma once
#include <variant>
#include <string>

#include "Expr.h"
#include "Token.h"

class Interpreter {
private:
    bool is_true(const Literal& e);
    std::string stringify(const Literal& object);
    bool isEqual(const Literal& left, const Literal& right);
    Literal interpret(const Expr& expr);
public:
    void execute(const Expr& expr);
};