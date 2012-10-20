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
    tinyclojure::Object* code = interpreter.parse("(+ 1 2)");
    interpreter.eval(code);
    
    delete code;
    
    return 0;
}

