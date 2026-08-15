#pragma once
#include <stdexcept>

class ParseError : public std::runtime_error {

    public:
        ParseError(const std::string& message) : std::runtime_error(message) {} // runtime requires a string in it's constructor
};