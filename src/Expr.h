#pragma once
#include <variant>
#include <memory>

#include "Token.h"


struct Binary;
struct Grouping;
struct Unary;

using Expr = std::variant<Binary, Grouping, Literal, Unary>;

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