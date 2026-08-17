#include "Interpreter.h"
#include <sstream>

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };

void Interpreter::execute(const Expr& expr) {

    Literal result = interpret(expr);
    std::cout << stringify(result) << "\n";
}

std::string Interpreter::stringify(const Literal& object) {
    if (std::holds_alternative<nullptr_t>(object)) {
        return "noto";
    }

    if (std::holds_alternative<bool>(object)) {
        bool type = std::get<bool>(object);
        return type ? "nete" : "leshano";
    }

    if (std::holds_alternative<std::string>(object)) {
        return std::get<std::string>(object);
    }

    if (std::holds_alternative<double>(object)) {
        std::stringstream ss;

        ss << std::get<double>(object);

        return ss.str();
    }

    return nullptr;
}

Literal Interpreter::interpret(const Expr& expr) {
    return std::visit(overload{

        [this](const Literal& node) -> Literal {
            return node;
        },

        [this](const Grouping& node) -> Literal {
            return interpret(*node.expression);
        },

        [this](const Unary& node) -> Literal {
            Literal right = interpret(*node.right); // recursively call interpret to return a naked Literal object/Expr syntax tree leave

            switch (node.op.type) {
                case TokenType::MINUS:
                    return -(std::get<double>(right));
                case TokenType::BANG:
                    return !is_true(right);
            }

            return nullptr;
        },

        [this](const Binary& node) -> Literal {
            Literal left = interpret(*node.left);
            Literal right = interpret(*node.right);

            switch (node.op.type) {
                case TokenType::MINUS:
                    return std::get<double>(left) - std::get<double>(right);
                case TokenType::SLASH:
                    return std::get<double>(left) / std::get<double>(right);
                case TokenType::STAR:
                    return std::get<double>(left) * std::get<double>(right);
                case TokenType::PLUS:
                    if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                        return std::get<std::string>(left) + std::get<std::string>(right);
                    }
                    return std::get<double>(left) + std::get<double>(right);
                case TokenType::LESS:
                    return std::get<double>(left) < std::get<double>(right);
                case TokenType::LESS_EQUAL:
                    return std::get<double>(left) <= std::get<double>(right);
                case TokenType::GREATER:
                    return std::get<double>(left) > std::get<double>(right);
                case TokenType::GREATER_EQUAL:
                    return std::get<double>(left) >= std::get<double>(right);
                case TokenType::EQUAL_EQUAL:
                    return isEqual(left, right);
                case TokenType::BANG_EQUAL:
                    return !isEqual(left, right);
                    
            }

            return nullptr;
        }
    }, expr);
}

bool Interpreter::isEqual(const Literal& left, const Literal& right) {
    return left == right;
}

bool Interpreter::is_true(const Literal& e) {
    // only NOTO(nil) and LESHANO(false) are false, rest is the truth
    if (std::holds_alternative<nullptr_t>(e)) return false;
    if (std::holds_alternative<bool>(e)) {
        return (!std::get<bool>(e));
    }

    return true;
}