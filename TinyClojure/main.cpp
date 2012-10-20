//
//  main.cpp
//  TinyClojure
//
//  Created by Duncan Steele on 20/10/2012.
//  Copyright (c) 2012 Slide to code. All rights reserved.
//

#include "TinyClojure.h"

#include <iostream>

int main(int argc, const char * argv[]) {
    tinyclojure::TinyClojure interpreter;
    tinyclojure::Object* code;
    
    try {
        code = interpreter.parse("(+ 1 2)");
    } catch (tinyclojure::ParserError error) {
        std::cout << error.position << ": " << error.message << std::endl;
        return 1;
    }

    if (code) {
        interpreter.eval(code);
    } else {
        std::cout << "null code\n";
        return 1;
    }
    
    return 0;
}

