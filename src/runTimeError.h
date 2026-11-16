#pragma once

#include <stdexcept>
#include <string>

struct RunTimeError : public std::runtime_error {
    Token token;
    RunTimeError(Token token, const std::string& message) : token(token), std::runtime_error(message) {} // runtime requires a string in it's constructor
};