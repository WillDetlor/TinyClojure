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
        tinyclojure::Object *code = interpreter.parse(input);
        if (code) {
            std::cout << "I read: " << code->stringRepresentation() << std::endl;
        }
    } while (input.length());
}

int main(int argc, const char * argv[]) {
    try {
        repl();
    } catch (tinyclojure::Error error) {
        std::cout << error.position << ": " << error.message << std::endl;
        return 1;
    }
    
    return 0;
}

