#pragma once
#include <variant>
#include <string>
#include <iostream>


#include "TokenType.h"
std::string to_string(TokenType type);
using Literal = std::variant<std::nullptr_t, double, std::string, bool>;

class Token {

    // overload stream extraction operator to print the token object
    friend std::ostream& operator<<(std::ostream& os, const Token& token);

    public:
        std::string lexeme;
        int line;
        TokenType type;
        Literal literal;
    
        Token(TokenType type, const std::string& lexeme, Literal literal, int line);
};

