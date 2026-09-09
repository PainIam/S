#include "Environment.h"
#include "runTimeError.h"

void Environment::define(const std::string& key, Literal value) {
    // can define a new variable or assign an existing
    values.insert_or_assign(key, value);
}

Literal Environment::get(const Token& name) {

    if (auto search = values.find(name.lexeme); search != values.end()) {
        return search->second;
    }

    throw RunTimeError(name, "ntho e sieo (undefined object/variable) ' " + name.lexeme + " '.");

}

void Environment::assign(const Token& name, Literal value) {

    if (values.contains(name.lexeme)) {
        values[name.lexeme] = value;
        return;
    }

    throw RunTimeError(name, "ntho e sieo (undefined object/variable) ' " + name.lexeme + " '.");
}