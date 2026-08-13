#pragma once

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Scanner.h"
#include "Token.h"

extern class S* uni_pointer;

class S {

    private:
        bool hadError = false;


    
        void report(int line, const std::string& where, const std::string& message) {
            std::cerr << "mola [" << line << "] Phoso ho " << where << ": " << message << "\n";
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
            std::exit(65);
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

            for (const auto& t : tokens) {
                std::cout << t << "\n";
            }
        }

        void error(int line, const std::string& message) {
            report(line, "", message);
        }

};