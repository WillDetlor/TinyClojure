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
     * class to represent Clojure objects
     */
    class Object {
    public:
        typedef enum {
            kObjectTypeString,
            kObjectTypeSymbol,
            kObjectTypeCons,
            kObjectTypeNil,
        } ObjectType;
        
        /// construct either a symbol (if symbol=true) or a string object otherwise
        Object(std::string stringValue, bool symbol=false);
        
        /// construct a nil object
        Object();
        
        /// construct a pair
        Object(Object *left, Object *right);
        
        /// construct a list
        Object(std::vector<Object*> list);
        
        ~Object();
        
        /// this object's type
        ObjectType type();
        
        /// return a reference to this object as a string value
        std::string& stringValue();
        
    protected:
        ObjectType _type;
        union {
            std::string* stringValue;
            struct {
                Object *left, *right;
            } consValue;
        } pointer;
    };
    
    /**
     * a wrapper for Object* when exporting any object
     *
     * this helps the garbage collector keep track of root objects still exist
     * TODO all objects exported from the TinyClojure should be exported via this
     */
    class ExportedObject {
    public:
    protected:
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
     * parts of the parser actually live in here
     */
    class ParserState {
    public:
        ParserState(std::string& stringin) : parserString(stringin) {
            position = 0;
        }
        
        std::string& parserString;
        int position;
                
        /**
         * skip newlines and whitespace.  A convenience wrapper for skipCharactersInString
         */
        int skipNewLinesAndWhitespace() {
            return skipCharactersInString(" \n\r\t");
        }
        
        /**
         * a clojure separator set
         */
        int skipSeparators() {
            return skipCharactersInString(" \t,\n\r");
        }
        
        /**
         * the character currently pointed to by the parser state
         */
        char currentChar() {
            return parserString[position];
        }
        
        /**
         * Safely peek ahead one character, returning 0 if this would run off the end of the string
         */
        char peekChar() {
            if (charactersLeft()) {
                return parserString[position];
            }
            
            return 0;
        }
        
        /**
         * return true when the state position is within the bounds of the string
         */
        bool charactersLeft() {
            return position < parserString.length();
        }
        
        /**
         * advance the parser through any characters in the passed string
         *
         * return the number of characters skipped
         */
        int skipCharactersInString(std::string skipSet) {
            int numberOfSkippedCharacters = 0;
            
            while (position < parserString.length()) {
                bool currentCharInSet = false;
                
                for (int skipSetIndex = 0; skipSetIndex < skipSet.length(); ++skipSetIndex) {
                    if (parserString[position] == skipSet[skipSetIndex]) {
                        currentCharInSet = true;
                        break;
                    }
                }
                
                if (currentCharInSet) {
                    ++position;
                    ++numberOfSkippedCharacters;
                } else {
                    break;
                }
                
                ++numberOfSkippedCharacters;
            }
            
            return numberOfSkippedCharacters;
        }
    };
    
    /**
     * Wrap a parser error to make it easy to throw an exception
     */
    class ParserError {
    public:
        ParserError(ParserState &state, std::string errorMessage) : message(errorMessage), position(state.position) {
            
        }
        
        std::string message;
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
        Object* registerObject(Object* object);
        
    protected:
        std::set<Object*> _objects;
    };
    
    class TinyClojure {
    public:
        /**
         * parse the passed string, returning the parsed objects, or NULL on error
         *
         * this will parse the data into a tree of S Expressions.  This throws an exception if there is a parser error.
         *
         * @return either an object created by parsing the input, or NULL if nothing was found
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
        
        /**
         * create a list from a std::vector
         *
         * this is here, not in the Object constructor because it needs access to the garbage collector
         */
        Object* listObject(std::vector<Object*> list);
        
        GarbageCollector *_gc;
        
        /// the internal recursive parser function, see parse for documentation
        Object* recursiveParse(ParserState& parseState);
    };
}

#endif /* defined(__TinyClojure__TinyClojure__) */