#pragma once

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Scanner.h"
#include "Token.h"
#include "Parser.h"
#include "AstPrinter.h"
#include "Interpreter.h"
#include "runTimeError.h"

extern class S* uni_pointer;

class S {

    private:
        bool hadError = false;
        bool hadRunTimeError = false;
        Interpreter interpreter = Interpreter();
    
        void report(int line, const std::string& where, const std::string& message) {
            std::cerr << "mola [" << line << "] Phoso " << where << ": " << message << "\n";
            hadError = true;
        }

    public:
        void runFile(const std::string& path) {
            std::ifstream file(path, std::ios::binary);

            if (!file) {
                std::cerr << "file not found\n" ;
                std::exit(2);
            }

            // read the entired buffer into a stringstream and return as a string
            std::stringstream ss;
            ss << file.rdbuf();

            run(ss.str());

            // the file had errors, 65 is an input error.
            if (hadError) std::exit(65);
            if (hadRunTimeError) std::exit(75);
        }
        
        void runPrompt() {
            std::cout << "S language v1.0, penya CTRL+D ho tsoa.\n";
            std::string line;

            while(true) {
                std::cout << "S >>> ";
                if (std::getline(std::cin, line)) {
                    run(line);
                    hadError = false; // prevent repl kicking you out and resetting ur program after a single mistake
                } else {
                    std::cout << "\n";
                    break;
                }
                
            }
        }

        void run(const std::string& source) {
            Scanner scanner = Scanner(source);
            std::vector<Token> tokens = scanner.scanTokens();
            Parser parser = Parser(tokens);
            Expr expression = parser.parse();

            if (hadError) return;

            interpreter.execute(expression);
            // std::cout << "\n";
        }

        void error(int line, const std::string& message) {
            report(line, "", message);
        }

        void error(Token token, const std::string& message) {
            if (token.type == TokenType::EOFF) {
                report(token.line, "qetellong", message);
            } else {
                report(token.line, "ho ' " + token.lexeme + "'", message);
            }
        }

        void runtimeError(RunTimeError e) {
            std::cout << e.what() << "\n [mola " << e.token.line << "]\n";
            hadRunTimeError = true;
        }

};