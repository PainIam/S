#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#include "Token.h"

class Scanner {
    private:
        int line = 1;
        int start = 0;
        int current = 0;
        std::string source;
        std::vector<Token> tokens;
        std::unordered_map<std::string, TokenType> keywords;
    public:
        Scanner(const std::string& s);

        std::vector<Token> scanTokens();
        bool isAtEnd();
        void scanToken();
        char advance();
        void addToken(TokenType type);
        void addToken(TokenType type, Literal literal);
        bool match(char nextChar);
        char peek();
        char peekNext();
        void string();
        bool isDigit(char c);
        void number();
        bool isAlpha(char c);
        bool isAlphaNumeric(char c);
        void identifier();

};