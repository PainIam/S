#pragma once
#include <variant>
#include <string>
#include <vector>

#include "Expr.h"
#include "Token.h"
#include "Stmt.h"

class Interpreter {
private:
    bool is_true(const Literal& e);
    std::string stringify(const Literal& object);
    bool isEqual(const Literal& left, const Literal& right);
    Literal interpret(const Expr& expr);
    void checkNumberOperands(const Token& op, const Literal& left, const Literal& right);
    void checkNumberOperands(const Token& op, const Literal& object);
    void visitStmt(const Stmt& stmt);
public:
    void execute(const std::vector<Stmt>& statements);
};