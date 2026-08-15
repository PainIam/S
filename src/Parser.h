#pragma once
#include <vector>
#include <initializer_list>
#include <memory>

#include "Expr.h"
#include "Token.h"
#include "ParseError.h"

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

        Expr expression();
        Expr equality();
        Expr comparison();
        Expr term();
        Expr factor();
        Expr unary();
        Expr primary();
        Token consume(TokenType type, const std::string& message);
        ParseError error(Token token, const std::string& message);

    public:
        Parser (const std::vector<Token>& tokens);
        Expr parse();

};