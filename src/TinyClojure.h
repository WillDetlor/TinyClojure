//
//  TinyClojure.h
//  TinyClojure
//
//  Created by Duncan Steele on 20/10/2012.
//  Copyright (c) 2012 Slide to code. All rights reserved.
//

#ifndef __TinyClojure__TinyClojure__
#define __TinyClojure__TinyClojure__

#include <iostream>
#include <string>
#include <set>
#include <vector>

namespace tinyclojure {
    /**
     * base class for all clojure values
     *
     * TODO create an exportable Object* wrapper that keeps track of which root objects are still alive for the GC
     */
    class Object {
        
    };
    
    /**
     * A class/interface for the interpreters IO
     *
     * Subclassing this allows for easier
     */
    class IOProxy {
    public:
        void writeOut(std::string stringout) {
            std::cout << stringout;
        }
        
        void writeErr(std::string stringout) {
            std::cerr << stringout;
        }
    protected:
        
    };
    
    /**
     * An object to represent the parser state
     *
     * construct it with the string to be parsed
     */
    class ParserState {
    public:
        ParserState(std::string& stringin) : parserString(stringin) {
            position = 0;
        }
        
        std::string& parserString;
        int position;
    };
    
    /**
     * a very simple garbage collector for TinyClojure objects
     */
    class GarbageCollector {
    public:
        GarbageCollector();
        ~GarbageCollector();
        
        /**
         * register an object with the garbage collector
         */
        void registerObject(Object* object);
        
    protected:
        std::set<Object*> _objects;
    };
    
    class TinyClojure {
    public:
        /**
         * parse the passed string, returning the parsed objects, or NULL on error
         *
         * this will parse the data into a tree of S Expressions
         *
         */
        Object* parse(std::string);
        
        /**
         * evaluate the code passed above
         */
        Object* eval(Object*);
        
        /**
         * default constructor
         */
        TinyClojure();
        
        /**
         * destructor
         */
        ~TinyClojure();
        
    protected:
        /// the IO proxy for this interpreter
        IOProxy *_ioProxy;
        GarbageCollector _gc;
    };
}

#endif /* defined(__TinyClojure__TinyClojure__) */
