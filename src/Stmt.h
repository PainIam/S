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
struct IfStmt;
struct WhileStmt;

using Stmt = std::variant<std::monostate, PrintStmt, ExprStmt, VarStmt, Block, IfStmt, WhileStmt>;

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

struct IfStmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;
};

struct WhileStmt {
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
};