#pragma once
#include <vector>
#include <initializer_list>
#include <memory>

#include "Expr.h"
#include "Token.h"
#include "runTimeError.h"
#include "Stmt.h"

class Parser {
    private:
        std::vector<Token> list;
        int current = 0;
        void sync();
        bool match(std::initializer_list<TokenType> types);
        bool check(TokenType type);
        bool isAtEnd();
        Token peek();
        Token previous();
        Token advance();

        Stmt statement();
        Stmt printStatement();
        Stmt exprStatement();
        Expr expression();
        Expr equality();
        Expr comparison();
        Expr term();
        Expr factor();
        Expr unary();
        Expr primary();
        Token consume(TokenType type, const std::string& message);
        RunTimeError error(Token token, const std::string& message);

    public:
        Parser (const std::vector<Token>& tokens);
        std::vector<Stmt> parse();

};