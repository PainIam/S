#include "Scanner.h"
#include "S.h"

// init the source string from the def constructor, this will feed the scanTokens function
Scanner::Scanner(const std::string& s) : source(s), keywords({
            {"le", TokenType::LE},
            {"haeba", TokenType::HAEBA}, 
            {"kapa", TokenType::KAPA},
            {"ho_seng_joalo", TokenType::HO_SENG_JOALO},
            {"nete", TokenType::NETE},
            {"leshano", TokenType::LESHANO},
            {"pheta", TokenType::PHETA},
            {"hafeela", TokenType::HAFEELA},
            {"ngola", TokenType::NGOLA}, 
            {"ntho", TokenType::NTHO},
            {"khutla", TokenType::KHUTLA}, 
            {"sehlopha", TokenType::SEHLOPA},
            {"noto", TokenType::NOTO}, 
            {"ena", TokenType::ENA},
            {"sebetsa", TokenType::SEBETSA}
}) {}



std::vector<Token> Scanner::scanTokens() {
    /*
    as i was coding this i asked myself why does this fun return a vector
    of tokens, yet we defined a vector already, then i remembered the vector
    itself is private in order to protect it from being altered from the language
    interface, (s.cc)
    */
    while(!isAtEnd()) {
        // scan the next lexeme
        start = current;
        scanToken();
    }

    tokens.push_back(Token (TokenType::EOFF, "", nullptr, line));
    return tokens;
}



void Scanner::scanToken() {
    // TODO
    // scan single char tokens
    char c = advance();
    switch(c) {
        case '(' : addToken(TokenType::LEFT_PAREN); break;
        case ')' : addToken(TokenType::RIGHT_PAREN); break;
        case '{' : addToken(TokenType::LEFT_BRACE); break;
        case '}' : addToken(TokenType::RIGHT_BRACE); break;
        case ',' : addToken(TokenType::COMMA); break;
        case '.' : addToken(TokenType::DOT); break;
        case '-' : addToken(TokenType::MINUS); break;
        case '+' : addToken(TokenType::PLUS); break;
        case ';' : addToken(TokenType::SEMICOLON); break;
        case '*' : addToken(TokenType::STAR); break; 
        case '!' : match('=') ? addToken(TokenType::BANG_EQUAL) : addToken(TokenType::BANG); break;
        case '=' : match('=') ? addToken(TokenType::EQUAL_EQUAL) : addToken(TokenType::EQUAL); break;
        case '>' : match('=') ? addToken(TokenType::GREATER_EQUAL) : addToken(TokenType::GREATER); break;
        case '<' : match('=') ? addToken(TokenType::LESS_EQUAL) : addToken(TokenType::LESS); break;
        case '\n':
            line++; break;
        case ' ':
        case '\t':
        case '\r':
            break;
        case '/' : 
            if (match('/')) {
                while (peek() != '\n' && !isAtEnd()) {
                    advance();
                }
            } else if (match('*')) {
                while (!isAtEnd() && !(peek() == '*' && peekNext() == '/')) {
                    advance();
                }


                // consume the * and /
                advance();
                advance();                

                // immediately throw an error if we got to the end without the comment being closed
                if (isAtEnd()) {
                    uni_pointer->error(line, "Tlhaloso e sa koaloang (unterminated comment)");
                }


            }
            else {
                addToken(TokenType::SLASH);
            }
            break;
        
        case '"': string(); break;


        default: 
            if (isDigit(c)) {
                number(); 
            } else if (isAlpha(c)) {
                identifier(); 
            } else {
                uni_pointer->error(line, "Tlhaka e sa lebelloang (An unexpected letter/character)");    
            }
            break;
            
    }
}

void Scanner::identifier() {
    while (isAlphaNumeric(peek())) advance();

    // check if the consumed word is a var or an identifier
    std::string text = source.substr(start, current - start);
    TokenType type;
    if (keywords.contains(text)) {
        type = keywords[text];
    } else {
        type = TokenType::IDENTIFIER;
    }

    addToken(type);
}

bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

void Scanner::number() {
    while (isDigit(peek())) advance();

    // floating points
    if (!isAtEnd() && (peek() == '.' && isDigit(peekNext()))) {
        advance();
        while (isDigit(peek())) advance();
    }

    std::string number_lexeme = source.substr(start, current - start);
    addToken(TokenType::NUMBER, std::stod(number_lexeme));
}

void Scanner::string() {

    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++; // allow multiline strings
        advance();
    }

    if (isAtEnd()) {
        uni_pointer->error(line, "mongolo o se nang qetello (unterminated string)"); 
    }

    // consume "
    advance();

    std::string trimmed_string = source.substr(start + 1, current - start - 2);
    addToken(TokenType::STRING, trimmed_string);

}

char Scanner::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}

char Scanner::peekNext() {
    if (current + 1 >= source.size()) return '\0';
    return source[current + 1];
}

bool Scanner::match(char nextChar) {
    if (isAtEnd()) return false;
    if (source[current] != nextChar) return false;

    current++;
    return true;
}

void Scanner::addToken(TokenType type) {
    addToken(type, nullptr);
}

void Scanner::addToken(TokenType type, Literal literal) {
    std::string literal_length = source.substr(start, current - start);
    tokens.push_back(Token(type, literal_length, literal, line));
}

bool Scanner::isAtEnd() {
    return current >= source.size();
}

char Scanner::advance() {
    // consumes next character and outputs it
    current++;
    return source[current - 1];
}