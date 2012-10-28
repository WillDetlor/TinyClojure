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
//  TinyClojure.h
//  TinyClojure
//
//  Created by Duncan Steele on 20/10/2012.
//

#ifndef __TinyClojure__TinyClojure__
#define __TinyClojure__TinyClojure__

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <cmath>
#include <vector>

namespace tinyclojure {
    /**
     * A fancy number class to implement Clojure's numeric tower
     */
    class Number {
    public:
        typedef enum {
            kNumberModeInteger,
            kNumberModeFloating,
        } NumberMode;
        
        Number(double val);
        Number(int val);
        Number();
        double floatingValue() const;
        Number floatingVersion() const;
        int integerValue() const;
        Number integerVersion() const;
        Number operator+(const Number& rhs) const;
        Number operator*(const Number& rhs) const;
        Number operator/(const Number& rhs) const;
        Number operator-(const Number& rhs) const;
        
        bool operator==(const Number& rhs) const;
        bool operator!=(const Number& rhs) const;
        bool operator<(const Number& rhs) const;
        bool operator>(const Number& rhs) const;
        bool operator<=(const Number& rhs) const;
        bool operator>=(const Number& rhs) const;
        

        /// return a string representation of this number
        std::string stringRepresentation() const;
    protected:
        static void equivalentModes(const Number& originalLHS, const Number& originalRHS, Number& newLHS, Number& newRHS);
        void setFloating(double val);
        void setInteger(int val);
                
        union {
            int integer;
            double floating;
        } _value;
        NumberMode _mode;
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

    /// a forward declaration to allow for an ExtensionFunction pointer in Object
    class ExtensionFunction;
    
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
            kObjectTypeNumber,
            kObjectTypeBoolean,
            kObjectTypeVector,
            kObjectTypeFunction,
        } ObjectType;
        
        /// construct either a symbol (if symbol=true) or a string object otherwise
        Object(std::string stringValue, bool symbol=false);
        
        /// construct a nil object
        Object();
        
        /// construct a boolean object
        Object(bool boolValue);
        
        /// construct an integer number object
        Object(int intValue);

        /// construct an floating point number object
        Object(double doubleValue);
        
        /// construct a number object
        Object(Number numberValue);
        
        /// construct a pair
        Object(Object *left, Object *right);
        
        /// construct a vector
        Object(std::vector<Object*> objects);
        
        /// construct a function
        Object(Object *code, std::vector<Object*> arguments);
        
        /// construct an extension function
        Object(ExtensionFunction *function);
        
        ~Object();
        
        /// given that this is an extension function, is this a builtin type?
        bool builtinFunction();
        
        /// this object's type
        ObjectType type() const { return _type; }
        
        /// negated equality operator
        bool operator!=(const Object& rhs);
        
        /// equality operator
        bool operator==(const Object& rhs);
        
        /// return a reference to this object as a string value
        std::string& stringValue();
        
        /// return a reference to this object as a vector
        std::vector<Object*>& vectorValue();
        
        /// accessor for code part of function value
        Object*& functionValueCode();
        
        /// accessor for extension function part of function value
        ExtensionFunction*& functionValueExtensionFunction();
        
        /// accessor for parameter list part of function value
        std::vector<Object*>& functionValueParameters();
        
        /// accessor for car
        Object*& consValueLeft();
        
        /// accessor for cdr
        Object*& consValueRight();
        
        /// return a reference to this object as an integer value
        Number& numberValue();
        
        /// return a reference to this object as a boolean value
        bool& booleanValue();
        
        /// this coerces whatever we have into a boolean
        bool coerceBoolean();
        
        /// build list, returning true and placing objects in the vector if it is a list, false otherwise
        bool buildList(std::vector<Object*>& results);

        /// build list, returning true if it is a list, false otherwise
        bool isList();
        
        /// build a string representation of the object
        std::string stringRepresentation(bool expandList=true);
        
    protected:
        ObjectType _type;
        union {
            std::string* stringValue;
            
            struct {
                Object *left, *right;
            } consValue;
            
            // if objectPointer is nil, interpret this as an extension function based function, else interpret as a user lambda
            struct {
                Object *objectPointer;
                std::vector<Object*>* argumentSymbols;
                ExtensionFunction *extensionFunctionPointer;
            } functionValue;
            
            std::vector<Object*>* vectorPointer;
            
            Number *numberPointer;
            
            bool booleanValue;
        } pointer;
    };
    
    
    /**
     * a very simple garbage collector for TinyClojure objects
     *
     * TODO nothing is really implemented yet
     *
     * the ExportedObject is essentially a C++ reference counting mechanism to keep track of "root objects" ie objects being used in the real world
     * when a garbage collection happens connectivity to these objects is the criteria for garbage collecting an object or not
     */
    class GarbageCollector {
    public:
        GarbageCollector();
        ~GarbageCollector();
        
        /**
         * register an object with the garbage collector
         */
        Object* registerObject(Object* object);
        
        
        /**
         * increment the "root object" reference count for this Object
         */
        Object* retainRootObject(Object *object);
        
        /**
         * decrement the "root object" reference count for this Object
         */
        Object* releaseRootObject(Object *object);
        
        /**
         * call this to start a garbage collection run
         */
        void collectGarbage();
        
    protected:
        std::set<Object*> _objects;
        std::map<Object*, int> _rootObjects;
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
        char peekChar();
        
        /**
         * Safely peek ahead two characters, returning 0 if this would run off the end of the string
         */
        char peekPeekChar();
        
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
        int skipCharactersInString(std::string skipSet);
    };
    
    /**
     * An object to represent the interpreter's state (probably more accurately, a single scope)
     */
    class InterpreterScope {
    public:
        /// construct a root scope
        InterpreterScope() : _parentScope(NULL) {
            
        }
        
        /// construct a scope from a parent scope
        InterpreterScope(InterpreterScope *parentScope) : _parentScope(parentScope) {
            
        }
        
        /// set the symbol in this scope
        void setSymbolInScope(std::string symbolName, Object *functionValue);
        
        /// return the symbol or NULL
        Object *lookupSymbolInScope(std::string symbolName);
                
        /// look for a symbol (in this and all parent scopes), return NULL if not found
        Object *lookupSymbol(std::string symbolName);
        
    protected:
        InterpreterScope *_parentScope;
        std::map<std::string, Object*> _symbolTable;
    };
    
    /**
     * Wrap a TinyClojure error to make it easy to throw an exception
     */
    class Error {
    public:
        /// constructor for parser errors
        Error(ParserState &state, std::string errorMessage) : message(errorMessage), position(state.position) {
            
        }
        
        /// constructor for generic errors
        Error(std::string errorMessage) : message(errorMessage) {
        }
        
        std::string message;
        int position;
    };
    
    /**
     * a wrapper for Object* when exporting any object
     *
     * this helps the garbage collector keep track of root objects still exist
     * DO NOT USE (read TODOs)
     *
     * TODO all objects exported from the TinyClojure should be exported via this
     * TODO right the garbage collector this references will be destroyed before this object is destroyed, ideally the garbage collector should be reference counted
     */
    class ExportedObject {
    public:
        ExportedObject(GarbageCollector *gc, Object* ptr) : _gc(gc), _object(ptr) {
            _gc->retainRootObject(_object);
        }
        
        ~ExportedObject() {
            _gc->releaseRootObject(_object);
        }
        
        Object& operator* () {
            return *_object;
        }
        
        Object* operator-> () {
            return _object;
        }
        
    protected:
        GarbageCollector *_gc;
        Object *_object;
    };
    
    /**
     * an abstract base class so that TinyClojure can provide callbacks to the ExtensionFunction objects
     */
    class Evaluator {
    public:
        /// the internal recursive evaluator, see eval for documentations
        virtual Object* recursiveEval(InterpreterScope *interpreterState, Object *code) = 0;
    };
    
    /**
     * An abstract base class for all interpreter functions
     *
     * Subclassing this class is the primary mechanism for extending the interpreter
     */
    class ExtensionFunction {
    public:        
        /// the name of this function
        virtual std::string functionName() { return ""; };
        
        /// the meat of the function happens in here, pass arguments and what it needs to evaluate
        virtual Object* execute(std::vector<Object*> arguments, InterpreterScope* interpreterState) {
            return NULL;
        }
        
        void setGarbageCollector(GarbageCollector *gc) {
            _gc = gc;
        }
        
        void setEvaluator(Evaluator *evaluator) {
            _evaluator = evaluator;
        }
        
        void setIOProxy(IOProxy *ioProxy) {
            _ioProxy = ioProxy;
        }
        
        /**
         * required number of arguments to this function
         *
         * return -1 if there is no restriction
         */
        virtual int requiredNumberOfArguments() {
            return -1;
        }
        
        /**
         * this function requires a number of arguments <= the return value of this function
         *
         * return -1 if there is no restriction
         */
        virtual int minimumNumberOfArguments() {
            return requiredNumberOfArguments();
        }
        
        /**
         * this function requires number of arguments >= the return value of this function
         *
         * return -1 if there is no restriction
         */
        virtual int maximumNumberOfArguments() {
            return requiredNumberOfArguments();
        }
        
    protected:
        GarbageCollector *_gc;
        Evaluator *_evaluator;
        IOProxy *_ioProxy;
    };
    
    class TinyClojure : Evaluator {
    public:
        /**
         * parse the passed string, returning the parsed objects, or NULL on error
         *
         * this will parse the data into a tree of S Expressions.  This throws an exception if there is a parser error.
         *
         * @return either an object created by parsing the input, or NULL if nothing was found
         */
        Object* parse(std::string stringin);
        
        /// parse all code in a string
        void parseAll(std::string codeText, std::vector<Object*>& expressions);
        
        /**
         * evaluate the code passed above
         */
        Object* eval(Object* object);
        
        /**
         * default constructor
         */
        TinyClojure();
        
        /**
         * destructor
         */
        ~TinyClojure();
        
        /// call this to load all extension functions, override it to change which functions are loaded
        virtual void loadExtensionFunctions();
        
        /// the internal recursive evaluator, see eval for documentations
        Object* recursiveEval(InterpreterScope *interpreterState, Object *code);
        
        /// this erases the persistent scope, removing all symbols and objects
        void resetInterpreter();
        
        /// add an extension function and reset the interpreter so that it is loaded
        void addExtensionFunction(ExtensionFunction *function);
        
    protected:
        /// add an extension function to the function table
        void internalAddExtensionFunction(ExtensionFunction *function);

        /// the IO proxy for this interpreter
        IOProxy *_ioProxy;
        
        /**
         * create a list from a std::vector
         *
         * this is here, not in the Object constructor because it needs access to the garbage collector
         */
        Object* listObject(std::vector<Object*> list);
        
        /// the base scope owned by this object and persistent between evaluations
        InterpreterScope *_baseScope;
        
        /// the shared garbage collector
        GarbageCollector *_gc;
        
        /// the internal recursive parser function, see parse for documentation
        Object* recursiveParse(ParserState& parseState);
        
        std::string _newlineSet, _excludeSet, _numberSet;
        
        /// a list of loaded extension functions
        std::vector<ExtensionFunction*> _extensionFunctions;
    };
}

#endif /* defined(__TinyClojure__TinyClojure__) */