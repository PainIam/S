#pragma once
#include <variant>
#include <memory>

#include "Token.h"


struct Binary;
struct Grouping;
struct Unary;
struct Var;
struct Assign;


using Expr = std::variant<Binary, Grouping, Literal, Unary, Var, Assign>;


struct Binary {
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
};

struct Grouping {
    std::unique_ptr<Expr> expression;
};

struct Unary {
    Token op;
    std::unique_ptr<Expr> right;
};

struct Var {
    Token name;
};

struct Assign {
    Token name;
    std::unique_ptr<Expr> value;
};
