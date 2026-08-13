// this file stores the list of keywords of the language, words reversed by the language itself
#pragma once
#include <cstdint>

enum class TokenType : uint8_t {

    // single character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // 1 or 2 character tokens
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // literals
    IDENTIFIER, STRING, NUMBER,

    // keywords
    LE, HAEBA ,KAPA, HO_SENG_JOALO, NETE, LESHANO,          // AND, IF, OR, ELIF, TRUE, FALSE
    PHETA, HAFEELA, NGOLA, NTHO, KHUTLA, SEHLOPA,           // WHILE, FOR, PRINT, VAR, RETURN, CLASS
    NOTO, ENA, SEBETSA,                                     // NULL, THIS, FUN(CTION)

    EOFF
};

