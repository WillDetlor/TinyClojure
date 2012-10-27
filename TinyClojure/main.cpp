//
//  main.cpp
//  TinyClojure
//
//  Created by Duncan Steele on 20/10/2012.
//  Copyright (c) 2012 Slide to code. All rights reserved.
//

#include "TinyClojure.h"

#include <iostream>

void repl() {
    std::string input;
    
    tinyclojure::TinyClojure interpreter;
    do {
        std::cout << "> ";
        std::getline(std::cin, input, '\n');

        try {
            tinyclojure::Object *code = interpreter.parse(input);
            if (code) {
                tinyclojure::Object *result = interpreter.eval(code);
                std::cout << result->stringRepresentation() << std::endl;
            }
        } catch (tinyclojure::Error error) {
            std::cout << error.position << ": " << error.message << std::endl << std::endl;
        }
    } while (input.length());
}

int main(int argc, const char * argv[]) {
    repl();
    
    return 0;
}

