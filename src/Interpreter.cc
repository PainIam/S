#include <sstream>

#include "Interpreter.h"
#include "runTimeError.h"
#include "S.h"

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };

void Interpreter::execute(const Expr& expr) {

try {    
    Literal result = interpret(expr);
    std::cout << stringify(result) << "\n";
} catch(RunTimeError error) {
    uni_pointer->runtimeError(error);
}
}

void Interpreter::checkNumberOperands(const Token& op, const Literal& left, const Literal& right) {
    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
        return;
    }

    throw RunTimeError(op, "Likarolo tsa tshebetso e tlameha ho ba palo tse peli (require both operands to be numbers)");
}

void Interpreter::checkNumberOperands(const Token& op, const Literal& object) {
    if (std::holds_alternative<double>(object)) return;

    throw RunTimeError(op, "ho hlokahala palo");
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
                    checkNumberOperands(node.op, left, right);
                    return std::get<double>(left) - std::get<double>(right);
                case TokenType::SLASH:
                    checkNumberOperands(node.op, left, right);
                    return std::get<double>(left) / std::get<double>(right);
                case TokenType::STAR:
                    checkNumberOperands(node.op, left, right);
                    return std::get<double>(left) * std::get<double>(right);
                case TokenType::PLUS:
                    if (std::holds_alternative<std::string>(left) && std::holds_alternative<std::string>(right)) {
                        return std::get<std::string>(left) + std::get<std::string>(right);
                    }
                    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right))
                        return std::get<double>(left) + std::get<double>(right);

                    throw RunTimeError(node.op, "Likarolo tsa tshebetso e tlameha ho ba palo tse peli kapa mantsoe a mabeli (2 numbers / 2 strings)");
                case TokenType::LESS:
                    checkNumberOperands(node.op, left, right);
                    return std::get<double>(left) < std::get<double>(right);
                case TokenType::LESS_EQUAL:
                    checkNumberOperands(node.op, left, right);
                    return std::get<double>(left) <= std::get<double>(right);
                case TokenType::GREATER:
                    checkNumberOperands(node.op, left, right);
                    return std::get<double>(left) > std::get<double>(right);
                case TokenType::GREATER_EQUAL:
                    checkNumberOperands(node.op, left, right);
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
        return (std::get<bool>(e));
    }

    return true;
}