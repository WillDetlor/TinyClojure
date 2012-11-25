// Copyright (C) 2012 Duncan Steele
// http://slidetocode.com
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//  tool.h
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
                if (code->type() != tinyclojure::Object::kObjectTypeNil) {
                    tinyclojure::Object *result = interpreter.eval(code);
                    std::cout << result->stringRepresentation() << std::endl;
                }
            }
        } catch (tinyclojure::Error error) {
            std::cout << error.position << ": " << error.message << std::endl << std::endl;
        }
    } while (!std::cin.eof());
}

int main(int argc, const char * argv[]) {
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

