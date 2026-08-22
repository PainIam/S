#pragma once
#include <variant>
#include <memory>

#include "Expr.h"

struct PrintStmt;
struct ExprStmt;

using Stmt = std::variant<PrintStmt, ExprStmt>;

struct ExprStmt {
    std::unique_ptr<Expr> expr;
};

struct PrintStmt {
    std::unique_ptr<Expr> expr;
};