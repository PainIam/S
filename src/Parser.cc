#include "Parser.h"
#include "S.h"

Parser::Parser(const std::vector<Token>& tokens) : list(tokens) {} // list of tokens is so self - explanatory

std::vector<Stmt> Parser::parse() {
    std::vector<Stmt> statements;
    while (!isAtEnd()) {
        statements.push_back(statement());
    }

    return statements;
}

Stmt Parser::statement() {
    // for now return either print or expr statement
    if (match({TokenType::NGOLA})) return printStatement();

    return exprStatement();
}

Stmt Parser::printStatement()  {

    Expr expr = expression();
    consume(TokenType::SEMICOLON, "lebelletse ';' kamora polelo");

    return Stmt{PrintStmt{ std::make_unique<Expr>(std::move(expr)) }};
}

Stmt Parser::exprStatement() {
    Expr expr = expression();

    consume(TokenType::SEMICOLON, "lebelletse ';' kamora polelo");

    return Stmt{ExprStmt{ std::make_unique<Expr>(std::move(expr)) }};
}

bool Parser::match(std::initializer_list<TokenType> types) {
    // checks if any 'types' corresponds with current type in list
    for (const auto type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) {
    // check if type equal to type in list without incrementing
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::isAtEnd() {
    return list[current].type == TokenType::EOFF;
}

Token Parser::peek() {
    // returns token[current]
    return list[current];
}

Token Parser::advance() {
    current++;
    return previous();
}

Token Parser::previous() {
    return list[current - 1];
}

Expr Parser::expression() {
    return equality();
}

Expr Parser::equality() {
    Expr expr = comparison();

    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token op = previous();
        Expr right = comparison();
        expr = Expr{Binary{ std::make_unique<Expr>(std::move(expr)), op, std::make_unique<Expr>(std::move(right)) }};
    }

    return expr;
}

Expr Parser::comparison() {
    Expr expr = term();

    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token op = previous();
        Expr right = term();
        expr = Expr{Binary{ std::make_unique<Expr>(std::move(expr)), op, std::make_unique<Expr>(std::move(right)) }};
    }

    return expr;
}

Expr Parser::term() {
    Expr expr = factor();

    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token op = previous();
        Expr right = factor();
        expr = Expr{Binary{ std::make_unique<Expr>(std::move(expr)), op, std::make_unique<Expr>(std::move(right)) }};
    }

    return expr;
}

Expr Parser::factor() {
    Expr expr = unary();

    while (match({TokenType::SLASH, TokenType::STAR})) {
        Token op = previous();
        Expr right = unary();
        expr = Expr{Binary{ std::make_unique<Expr>(std::move(expr)), op, std::make_unique<Expr>(std::move(right)) }};
    }

    return expr;
}

Expr Parser::unary() {
    // interesting case, we should recursively call this function as it's grammar
    // unary       → ( "!" | "-" ) unary | primary ; suggests
    // after finding the op, we call ourselves again to get a primary expression, or even if we found no op, primary will be required either way
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token op = previous();
        Expr right = unary();
        return Expr{Unary{ op, std::make_unique<Expr>(std::move(right)) }};
    }
    return primary();
}

Expr Parser::primary() {
    if (match({TokenType::LESHANO})) return Expr{Literal{false}};
    if (match({TokenType::NETE}))    return Expr{Literal{true}};
    if (match({TokenType::NOTO}))    return Expr{Literal{nullptr}};
    
    if (match({TokenType::STRING, TokenType::NUMBER})) {
        return Expr{Literal{previous().literal}};
    }

    if (match({TokenType::LEFT_PAREN})) {
        Expr expr = expression();
        consume(TokenType::RIGHT_PAREN, "lebelletsoe ')' ka mor'a polelo.");
        return Expr{Grouping{std::make_unique<Expr>(std::move(expr))}};
    }

    // fail safe (there is not expression, atleast a valid one)
    throw error(peek(), "lebelletsoe polelo");
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();

    throw error(peek(), message);
}

RunTimeError Parser::error(Token token, const std::string& message) {
    // returns a throwable and calls S.h's error overloaded f:n
    uni_pointer->error(token, message);
    return RunTimeError(token, message);
}

void Parser::sync() {
    
    // in case of an error we discard tokens till we get to the beginning of a statement
    advance();
    while(!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return; // if the previously consumed token(first statement in sync) was a ';' then we are ready to begin a new statement

        // consume until we get to a declarative statement or a print or if/while/for -- basically 
        switch(previous().type) {
            case TokenType::NTHO:
            case TokenType::HAEBA:
            case TokenType::PHETA:
            case TokenType::HAFEELA:
            case TokenType::SEBETSA:
            case TokenType::SEHLOPA:
            case TokenType::KHUTLA:
            case TokenType::NGOLA:
                return;
        }

        advance(); // we are still not the beginning of a valid statement
    }
}