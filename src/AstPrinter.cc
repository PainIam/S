#include "AstPrinter.h"

#include <iostream>
#include <variant>

// The Overload Pattern Boilerplate
template<class... Ts> struct overload : Ts... { using Ts::operator()...; };

// a pretty printer for debugging purposes

void prettyPrint(const Expr& expr) {
    std::visit(overload{

        [](const Literal& node){ std::visit([](auto& v){ std::cout << v; }, node); },
        [](const Binary& node) {
            std::cout << "(";
            std::cout << node.op.lexeme << " ";
            prettyPrint(*node.left);
            std::cout << " ";
            prettyPrint(*node.right);
            std::cout << ")";
        },
        [](const Grouping& node){
            std::cout << "(";
            prettyPrint(*node.expression);
            std::cout << ")";
        },
        [](const Unary& node) {
            std::cout << "(";
            std::cout << node.op.lexeme << " ";
            prettyPrint(*node.right);
            std::cout << ")";
        }
    }, expr);
}