#include "Parser.h"
#include "S.h"

#include <variant>

Parser::Parser(const std::vector<Token>& tokens) : list(tokens) {} // list of tokens is so self - explanatory

std::vector<Stmt> Parser::parse() {
    std::vector<Stmt> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }

    return statements;
}

Stmt Parser::declaration() {
    try {
        if (match({TokenType::NTHO})) return varDeclaration();

        return statement();
    } catch (RunTimeError e) {
        if (peek().type != TokenType::EOFF)
            sync();
        return std::monostate{};
    }
}

Stmt Parser::varDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "lebeletsoe lebitso la ntho(object / variable name)");

    Expr ini = nullptr;
    if (match({TokenType::EQUAL})) {
        ini = expression();
    }
    consume(TokenType::SEMICOLON, "lebelletsoe ';' kamora polelo");
    return Stmt{VarStmt{ name, std::make_unique<Expr>(std::move(ini))}};

}

Stmt Parser::statement() {
    if (match({TokenType::HAEBA})) return ifStatement();
    if (match({TokenType::PHETA_HA})) return whileStatement();
    if (match({TokenType::HAFEELA})) return forStatement();
    if (match({TokenType::NGOLA})) return printStatement();
    if (match({TokenType::LEFT_BRACE})) return Stmt{Block{block()}};

    return exprStatement();
}

Stmt Parser::whileStatement() {
    consume(TokenType::LEFT_PAREN, "lebelletsoe '(' kamora 'pheta'");
    Expr condition = expression();
    consume(TokenType::RIGHT_PAREN, "lebelletsoe ')' kamora polelo ea 'pheta'");

    Stmt body = statement(); // consume preceding statements

    return Stmt{WhileStmt{std::make_unique<Expr>(std::move(condition)), std::make_unique<Stmt>(std::move(body))}};

}

Stmt Parser::forStatement() {
    /*
        syntactic sugar for the for loop
        wrap the statements to be exucuted by the loop and the incremental statement in to
        a block statement, respectively. if condition is not null, wrap the condition and the
        already existing block into a whileStmt, if there exists a declaration or initialization
        wrap it in a another block statement consisting of the declaration and the whileStmt
        and return the block of blocks, it surely cannot get more abstract than this
    */
    consume(TokenType::LEFT_PAREN, "lebelletsoe '(' kamora 'hafeela'");

    // check if there exists an initializer 
    Stmt initializer {};
    if (match({TokenType::SEMICOLON})) {
        initializer = std::monostate{};
    } else if (match({TokenType::NTHO})) {
        initializer = varDeclaration();
    } else {
        initializer = exprStatement();
    }

    Expr condition = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "lebelletsoe ';' kamora polelo ea 'hafeela'");

    Expr increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN)) {
        increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "lebelletsoe ';' kamora polelo tsa 'hafeela'");

    Stmt body = statement();


    if (!std::holds_alternative<Literal>(increment) ||
        !std::holds_alternative<std::nullptr_t>(std::get<Literal>(increment))) {
            // increment is NOT empty — proceed with folding it in
            Stmt incrStmt = ExprStmt { std::make_unique<Expr>(std::move(increment)) };

            std::vector<Stmt> v;
            v.push_back(std::move(body));
            v.push_back(std::move(incrStmt));

            body = Block { std::move(v) }; 
    }

    if (std::holds_alternative<Literal>(condition) &&
        std::holds_alternative<nullptr_t>(std::get<Literal>(condition))) {
            condition = Expr{Literal{true}};
        }

    body = WhileStmt{
        std::make_unique<Expr>(std::move(condition)),
        std::make_unique<Stmt>(std::move(body)) // statement and the incremental, respectively
    };

    // precede the body with an initializer(if any) followed by the stmt + incremental
    if (!std::holds_alternative<std::monostate>(initializer)) {
        std::vector<Stmt> v;
        v.push_back(std::move(initializer));
        v.push_back(std::move(body));
        body = Block { std::move(v) };
    }

    return body;
}


Stmt Parser::ifStatement() {
    consume(TokenType::LEFT_PAREN, "lebelletsoe '(' kamora 'haeba'");
    Expr condition = expression();
    consume(TokenType::RIGHT_PAREN, "lebelletsoe ')' kamora polelo ea 'haeba'");

    Stmt thenBrach = statement();
    Stmt elseBranch = std::monostate{};
    if (match({TokenType::HO_SENG_JOALO})) {
        elseBranch = statement();
    }

    return Stmt { IfStmt 
                    {
                        std::make_unique<Expr>(std::move(condition)), 
                        std::make_unique<Stmt>(std::move(thenBrach)),
                        std::make_unique<Stmt>(std::move(elseBranch))
                    } 
                };
}

std::vector<Stmt> Parser::block() {
    std::vector<Stmt> statements;

    while(!check(TokenType::RIGHT_BRACE) && !isAtEnd()) { // explicit isatend() check to avoid inf loop incase of no closing brace
        statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "lebelletse '}' ");
    return statements;
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
    return assignment();
}

Expr Parser::assignment() {

    // get var.expr
    Expr expr = logical_or();

    if (match({TokenType::EQUAL})) {
        Token token = previous();
        Expr value = assignment(); // recursively call assignment, which should evaluate the expr without getting in this code block again

        if (std::holds_alternative<Var>(expr)) {
            Token name = std::get<Var>(expr).name;

            return Expr{Assign{name, std::make_unique<Expr>(std::move(value))}};
        }

        error(token, "Ntho e abeloang ha ea nepahala."); // if expr is not a variable then we are assigning to some bs

    }

    return expr;
}

Expr Parser::logical_or() {
    Expr expr = logical_and();

    while (match({TokenType::LE, TokenType::KAPA})) {
        Token op = previous();
        Expr left = logical_and();
        expr = Expr{Logical{std::make_unique<Expr>(std::move(expr)), op, std::make_unique<Expr>(std::move(expr))}};
    }

    return expr;
}

Expr Parser::logical_and() {
    Expr expr = equality();

    while (match({TokenType::LE, TokenType::KAPA})) {
        Token op = previous();
        Expr left = equality();
        expr = Expr{Logical{std::make_unique<Expr>(std::move(expr)), op, std::make_unique<Expr>(std::move(expr))}};
    }

    return expr;
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

    if (match({TokenType::IDENTIFIER})) {
        return Expr{Var{previous()}};
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
            case TokenType::PHETA_HA:
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