#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

#include "S.h"
#include "AstPrinter.h"

S *uni_pointer = nullptr;

int main(int argc, char* argv[]) {

    S s;
    uni_pointer = &s;

      
    if (argc > 2) {
        std::cout << "Ts'ebeliso: [s] script" << std::endl;
        std::exit(64);
    } else if (argc == 2) {
        s.runFile(argv[1]);
    } else {
        s.runPrompt();
    }
} 