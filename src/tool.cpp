//
//  main.cpp
//  TinyClojure
//
//  Created by Duncan Steele on 20/10/2012.
//  Copyright (c) 2012 Slide to code. All rights reserved.
//

#include "TinyClojure.h"

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>


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
//    argc=1;
    
    bool startRepl = false;
    
    if (argc == 1) {
        startRepl = true;
    }
    
    // execute any files passed on the command line
    int argpos = 1;
    while (argpos < argc) {
        std::string filename(argv[argpos]);
        
        if (filename=="-h") {
            std::cout << "help: -h prints this message, -r starts the repl, pass any files to execute" << std::endl;
            startRepl = false;
        } else if (filename=="-r") {
            // guarantee that the repl starts
            startRepl = true;
        }
        
        std::ifstream t(filename);
        std::string fileInput((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());
        
        try {
            tinyclojure::TinyClojure interpreter;
            std::vector<tinyclojure::Object*> expressions;
            interpreter.parseAll(fileInput, expressions);
            
            for (int expressionIndex = 0; expressionIndex < expressions.size(); ++expressionIndex) {
                interpreter.eval(expressions[expressionIndex])->stringRepresentation();
            }            
        } catch (tinyclojure::Error error) {
            std::cout << error.position << ": " << error.message << std::endl << std::endl;
        }
        
        ++argpos;
    }
    
    if (startRepl) {
        repl();
    }
    
    return 0;
}

