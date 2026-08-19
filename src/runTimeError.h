#pragma once
#include <stdexcept>

class RunTimeError : public std::runtime_error {

    public:
        Token token;
        RunTimeError(Token token, const std::string& message) : token(token), std::runtime_error(message) {} // runtime requires a string in it's constructor
};