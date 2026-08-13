#include "Token.h"

Token::Token (TokenType type, const std::string& lexeme, Literal literal, int line) :
            type(type),
            lexeme(lexeme),
            literal(literal),
            line(line)
            {

            }


std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << to_string(token.type) << " " << token.lexeme << " ";

    std::visit([&os](const auto& value){
        os << value << "\n";
    }, token.literal);

    return os;
}

// allow ostream to print the tokentypes, not as enums but literal strings 
std::string to_string(TokenType type) {

    std::string str;
    switch(static_cast<int>(type)) {
        case 0 : str = "LEFT_PAREN"; break;
        case 1 : str = "RIGTH_PAREN"; break;
        case 2 : str = "LEFT_BRACE"; break;
        case 3 : str = "RIGHT_BRACE"; break;
        case 4 : str = "COMMA"; break;
        case 5 : str = "DOT"; break;
        case 6 : str = "MINUS"; break;
        case 7 : str = "PLUS"; break;
        case 8 : str = "SEMICOLON"; break;
        case 9 : str = "SLASH"; break;
        case 10 : str = "STAR"; break;

        case 11 : str = "BANG"; break;
        case 12 : str = "BANG_EQUAL"; break;
        case 13 : str = "EQUAL"; break;
        case 14 : str = "EQUAL_EQUAL"; break;
        case 15 : str = "GREATER"; break;
        case 16 : str = "GREATER_EQUAL"; break;
        case 17 : str = "LESS"; break;
        case 18 : str = "LESS_EQUAL"; break;

        case 19 : str = "IDENTIFIER"; break;
        case 20 : str = "STRING"; break;
        case 21 : str = "NUMBER"; break;
        
        case 22 : str = "LE"; break;
        case 23 : str = "HAEBA"; break;
        case 24 : str = "KAPA"; break;
        case 25 : str = "HO_SENG_JOALO"; break;
        case 26 : str = "NETE"; break;
        case 27 : str = "LESHANO"; break;
        case 28 : str = "PHETA"; break;
        case 29 : str = "HAFEELA"; break;
        case 30 : str = "NGOLA"; break;
        case 31 : str = "NTHO"; break;
        case 32 : str = "KHUTLA"; break;
        case 33 : str = "SEHLOPHA"; break;
        case 34 : str = "NOTO"; break;
        case 35 : str = "ENA"; break;
        case 36 : str = "SEBETSA"; break;
        
        default: str = "EOF"; break;
    }

    return str;
}