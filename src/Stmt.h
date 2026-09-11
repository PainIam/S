#pragma once
#include <variant>
#include <memory>
#include <vector>

#include "Expr.h"
#include "Token.h"

struct PrintStmt;
struct ExprStmt;
struct VarStmt;
struct Block;

using Stmt = std::variant<std::monostate, PrintStmt, ExprStmt, VarStmt, Block>;

struct ExprStmt {
    std::unique_ptr<Expr> expr;
};

struct PrintStmt {
    std::unique_ptr<Expr> expr;
};

struct VarStmt {
    Token name;
    std::unique_ptr<Expr> ini;
};

struct Block {
    std::vector<Stmt> statements;
};