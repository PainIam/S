#pragma once
#include <variant>
#include <string>
#include <memory>
#include <vector>

#include "Expr.h"
#include "Token.h"
#include "Stmt.h"
#include "Environment.h"

class Interpreter {
private:
    bool is_true(const Literal& e);
    std::string stringify(const Literal& object);
    bool isEqual(const Literal& left, const Literal& right);
    Literal interpret(const Expr& expr);
    void checkNumberOperands(const Token& op, const Literal& left, const Literal& right);
    void checkNumberOperands(const Token& op, const Literal& object);
    void visitStmt(const Stmt& stmt);
    std::shared_ptr<Environment> environment;
    void executeBlock(const std::vector<Stmt>& statements, std::shared_ptr<Environment> env);

public:
    Interpreter();
    Interpreter(const Interpreter& interpreter) = delete;
    Interpreter& operator=(const Interpreter& interpreter) = delete;
    ~Interpreter();
    void execute(const std::vector<Stmt>& statements);

};