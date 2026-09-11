#include "Environment.h"
#include "runTimeError.h"


Environment::Environment() {
    enclosing = nullptr;
}

Environment::Environment(Environment& enclose) : enclosing(&enclose) {}

void Environment::define(const std::string& key, Literal value) {

    values.insert({key, value});
}

Literal Environment::get(const Token& name) {

    if (auto search = values.find(name.lexeme); search != values.end()) {
        return search->second;
    }

    // recursively call parent environment till we get to the global env
    // we search each parent env for the "name.lexeme(variable name)"
    // we search till the root env, if variable is not found, then can we 
    // throw the runtimeerrror that we did not find the said variable name
    if (enclosing != nullptr) return enclosing->get(name);


    throw RunTimeError(name, "ntho e sieo (undefined object/variable) ' " + name.lexeme + " '.");

}

void Environment::assign(const Token& name, Literal value) {
    // only redefine a variable when it exists in the current / parent environments
    
    // check current env
    if (values.contains(name.lexeme)) {
        values[name.lexeme] = value;
        return;
    }

    // call parent env, follows same idea as get 
    if (enclosing != nullptr) enclosing->assign(name, value);

    throw RunTimeError(name, "ntho e sieo (undefined object/variable) ' " + name.lexeme + " '.");
}