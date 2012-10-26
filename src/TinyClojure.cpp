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
//  TinyClojure.cpp
//  TinyClojure
//
//  Created by Duncan Steele on 20/10/2012.
//

#include "TinyClojure.h"
#include <sstream>

namespace tinyclojure {
    
#pragma mark -
#pragma mark Standard Library
    
    namespace core {
        /// function for adding a list of numbers
        class Arithmetic : public ExtensionFunction {
            virtual double floatOperation(double lhs, double rhs) {
                return 0;
            }

            virtual int integerOperation(int lhs, int rhs) {
                return 0;
            }
            
            int minimumNumberOfArguments() {
                return 1;
            }
            
            Object* execute(std::vector<Object*> arguments, InterpreterScope* interpreterState) {
                int *values = new int[arguments.size()];
                std::vector<Object*> evaluatedArguments;
                for (int argumentIndex = 0; argumentIndex < arguments.size(); ++argumentIndex) {
                    Object *evaluatedArgument = _gc->registerObject(_evaluator->recursiveEval(interpreterState, arguments[argumentIndex]));
                    if (evaluatedArgument->type() != Object::kObjectTypeInteger) {
                        throw Error("Arithmetic functions require all arguments to be numbers");
                    }
                    values[argumentIndex] = evaluatedArgument->integerValue();
                }
                
                int current = values[0];
                for (int argumentIndex = 1; argumentIndex < arguments.size(); ++argumentIndex) {
                    current = integerOperation(current, values[argumentIndex]);
                }
                delete[] values;

                return _gc->registerObject(new Object(current));
            }
        };
        
        class Plus : public Arithmetic {
            std::string functionName() {
                return std::string("+");
            }
            
            virtual double floatOperation(double lhs, double rhs) {
                return lhs + rhs;
            }
            
            virtual int integerOperation(int lhs, int rhs) {
                return lhs + rhs;
            }
        };
        
        class Minus : public Arithmetic {
            std::string functionName() {
                return "-";
            }
            
            virtual double floatOperation(double lhs, double rhs) {
                return lhs - rhs;
            }
            
            virtual int integerOperation(int lhs, int rhs) {
                return lhs - rhs;
            }
        };
        
        class Multiply : public Arithmetic {
            std::string functionName() {
                return "*";
            }
            
            virtual double floatOperation(double lhs, double rhs) {
                return lhs * rhs;
            }
            
            virtual int integerOperation(int lhs, int rhs) {
                return lhs * rhs;
            }
        };
        
        class Divide  : public Arithmetic {
            std::string functionName() {
                return "/";
            }
            
            virtual double floatOperation(double lhs, double rhs) {
                return lhs / rhs;
            }
            
            virtual int integerOperation(int lhs, int rhs) {
                return lhs / rhs;
            }
        };
        
        class If : public ExtensionFunction {
            std::string functionName() {
                return "if";
            }
            
            int minimumNumberOfArguments() {
                return 2;
            }
            
            int maximumNumberOfArguments() {
                return 3;
            }
            
            Object *execute(std::vector<Object*> arguments, InterpreterScope* interpreterState) {
                Object  *condition = arguments[0],
                        *trueBranch = arguments[1],
                        *falseBranch = NULL;
                
                if (arguments.size()==3) {
                    falseBranch = arguments[2];
                } else {
                    falseBranch = _gc->registerObject(new Object());
                }
                
                Object *evaluatedCondition = _evaluator->recursiveEval(interpreterState, condition);
                if (evaluatedCondition->coerceBoolean()) {
                    return _evaluator->recursiveEval(interpreterState, trueBranch);
                } else {
                    return _evaluator->recursiveEval(interpreterState, falseBranch);
                }
            }
        };
        
        class Equality : public ExtensionFunction {
            std::string functionName() {
                return "=";
            }
            
            int minimumNumberOfArguments() {
                return 1;
            }
            
            Object *execute(std::vector<Object*> arguments, InterpreterScope* interpreterState) {
                Object *lhs = _evaluator->recursiveEval(interpreterState, arguments[0]);
                
                for (int argumentIndex = 1; argumentIndex < arguments.size(); ++argumentIndex) {
                    Object *rhs = _evaluator->recursiveEval(interpreterState, arguments[argumentIndex]);
                    
                    if (*lhs!=*rhs) {
                        return _gc->registerObject(new Object(false));
                    }
                }
                
                return _gc->registerObject(new Object(true));
            }
        };
        
        class NumericInequality : public ExtensionFunction {
            Object *execute(std::vector<Object*> arguments, InterpreterScope* interpreterState) {
                std::vector<Object*> evaluatedArguments;
                for (int argumentIndex=0; argumentIndex<arguments.size(); ++argumentIndex) {
                    evaluatedArguments.push_back(_evaluator->recursiveEval(interpreterState, arguments[argumentIndex]));
                    
                    if (evaluatedArguments.back()->type() != Object::kObjectTypeInteger) {
                        std::stringstream stringBuilder;
                        
                        stringBuilder   << "Arguments to "
                                        << functionName()
                                        << " must all be numeric";
                        
                        throw Error(stringBuilder.str());
                    }
                }

                Object *lhs = _evaluator->recursiveEval(interpreterState, arguments[0]);
                
                for (int argumentIndex = 1; argumentIndex < arguments.size(); ++argumentIndex) {
                    Object *rhs = _evaluator->recursiveEval(interpreterState, arguments[argumentIndex]);
                    
                    if (!comparison(lhs->integerValue(), rhs->integerValue())) {
                        return _gc->registerObject(new Object(false));
                    }
                }
                
                return _gc->registerObject(new Object(true));
            }
            
        protected:
            bool comparison(int lhs, int rhs) {
                return true;
            }
        };
        
        class LessThan : public ExtensionFunction {
            std::string functionName() {
                return "<";
            }
            
            bool comparison(int lhs, int rhs) {
                return lhs < rhs;
            }
        };

        class GreaterThan : public ExtensionFunction {
            std::string functionName() {
                return ">";
            }
            
            bool comparison(int lhs, int rhs) {
                return lhs > rhs;
            }
        };

        class LessThanOrEqual : public ExtensionFunction {
            std::string functionName() {
                return "<=";
            }
            
            bool comparison(int lhs, int rhs) {
                return lhs <= rhs;
            }
        };

        class GreaterThanOrEqual : public ExtensionFunction {
            std::string functionName() {
                return ">=";
            }
            
            bool comparison(int lhs, int rhs) {
                return lhs >= rhs;
            }
        };
        
        class Vector : public ExtensionFunction {
            std::string functionName() {
                return "vector";
            }
            
            Object* execute(std::vector<Object*> arguments, InterpreterScope *interpreterState) {
                std::vector<Object*> evaluatedArguments;
                for (int argumentIndex=0; argumentIndex<arguments.size(); ++argumentIndex) {
                    evaluatedArguments.push_back(_evaluator->recursiveEval(interpreterState, arguments[argumentIndex]));
                }
                return _gc->registerObject(new Object(evaluatedArguments));
            }
        };
        
        class List : public ExtensionFunction {
            std::string functionName() {
                return "list";
            }
            
            Object *execute(std::vector<Object*> arguments, InterpreterScope *interpreterState) {
                std::vector<Object*> evaluatedArguments;
                for (int argumentIndex=0; argumentIndex<arguments.size(); ++argumentIndex) {
                    evaluatedArguments.push_back(_evaluator->recursiveEval(interpreterState, arguments[argumentIndex]));
                }
                
                Object *null = _gc->registerObject(new Object());

                if (arguments.size()==0) {
                    return _gc->registerObject(new Object(null,null));
                } else if (arguments.size()==1) {
                    return _gc->registerObject(new Object(evaluatedArguments[0],null));
                } else {
                    Object *rhs = null;
                    
                    for (long argumentIndex = evaluatedArguments.size()-1; argumentIndex >= 0; --argumentIndex) {
                        Object *lhs = evaluatedArguments[argumentIndex];
                        
                        rhs = _gc->registerObject(new Object(lhs, rhs));
                    }
                    
                    return rhs;
                }
            }
        };
        
        class Fn : public ExtensionFunction {
            std::string functionName() {
                return "fn";
            }
            
            int minimumNumberOfArguments() {
                return 2;
            }
            
            Object *execute(std::vector<Object*> arguments, InterpreterScope *interpreterState) {
                Object * arglist = arguments[0];
                
                // remove the initial argument, just leaving the function body
                arguments.erase(arguments.begin());
                
                // now run through the lambda capturing the objects
                std::vector<Object*> transformedArguments;
                
                return _gc->registerObject(new Object(0));
            }
        };
        
        class Cons : public ExtensionFunction {
            std::string functionName() {
                return "cons";
            }
            
            int requiredNumberOfArguments() {
                return 2;
            }
            
            Object *execute(std::vector<Object*> arguments, InterpreterScope *interpreterState) {
                Object  *lhs = _evaluator->recursiveEval(interpreterState, arguments[0]),
                        *rhs = _evaluator->recursiveEval(interpreterState, arguments[1]);
                
                return _gc->registerObject(new Object(lhs, rhs));
            }
        };
        
        class Print : public ExtensionFunction {
            std::string functionName() {
                return "print";
            }
            
            Object *execute(std::vector<Object*> arguments, InterpreterScope *interpreterState) {
                for (int argumentIndex=0; argumentIndex<arguments.size(); ++argumentIndex) {
                    Object *evaluatedArgument = _evaluator->recursiveEval(interpreterState, arguments[argumentIndex]);
                    
                    std::string thisArgument = evaluatedArgument->stringRepresentation();
                    
                    _ioProxy->writeOut(thisArgument);
                    _ioProxy->writeOut("\n");
                }
                
                return NULL;
            }
        };
        
        class Def : public ExtensionFunction {
            std::string functionName() {
                return "def";
            }
            
            int requiredNumberOfArguments() {
                return 2;
            }
            
            Object *execute(std::vector<Object*> arguments, InterpreterScope *interpreterState) {
                Object  *symbol = arguments[0],
                        *value = _evaluator->recursiveEval(interpreterState, arguments[1]);
                
                if (symbol->type()!=Object::kObjectTypeSymbol) {
                    throw Error("first argument to def must be a symbol");
                }
                
                interpreterState->setSymbolInScope(symbol->stringValue(), value);
                
                return _gc->registerObject(new Object());
            }
        };
        
        class Do : public ExtensionFunction {
            std::string functionName() {
                return "do";
            }
            
            Object *execute(std::vector<Object*> arguments, InterpreterScope *interpreterState) {
                Object  *retValue = _gc->registerObject(new Object());
                
                for (int argumentIndex = 0; argumentIndex < arguments.size(); ++argumentIndex) {
                    retValue = _evaluator->recursiveEval(interpreterState, arguments[argumentIndex]);
                }
                
                return retValue;
            }
        };
    }
    
#pragma mark -
#pragma mark InterpreterScope
    
    Object* InterpreterScope::lookupSymbolInScope(std::string symbolName) {
        std::map<std::string, Object*>::iterator it = _symbolTable.find(symbolName);
        
        if (it == _symbolTable.end()) {
            return NULL;
        } else {
            return it->second;
        }
    }
    
    void InterpreterScope::setSymbolInScope(std::string symbolName, Object *objectValue) {
        _symbolTable[symbolName] = objectValue;
    }
    
    Object* InterpreterScope::lookupSymbol(std::string symbolName) {
        Object *ret = lookupSymbolInScope(symbolName);
        if (ret) {
            return ret;
        } else {
            if (_parentScope) {
                return _parentScope->lookupSymbol(symbolName);
            } else {
                return NULL;
            }
        }
    }


#pragma mark -
#pragma mark Object
    
    Object::Object() {
        _type = kObjectTypeNil;
    }
    
    Object::Object(std::string stringVal, bool symbol) {
        if (symbol) {
            _type = kObjectTypeSymbol;
            pointer.stringValue = new std::string(stringVal);            
        } else {
            _type = kObjectTypeString;
            pointer.stringValue = new std::string(stringVal);
        }
    }
    
    Object::~Object() {
        switch (_type) {
            case kObjectTypeSymbol:
            case kObjectTypeString:
                delete pointer.stringValue;
                break;
                
            case kObjectTypeVector:
                delete pointer.vectorPointer;
                break;
            
            case kObjectTypeCons:
                // it isn't our business deleting "unused" objects, that is for the GC
            case kObjectTypeInteger:
            case kObjectTypeBoolean:
            case kObjectTypeNil:
                // nothing need be done
                break;
        }
    }
    
    bool Object::operator==(const Object& rhs) {
        if (type() != rhs.type()) {
            return false;
        }
        
        switch (_type) {
            case kObjectTypeBoolean:
                return pointer.booleanValue == rhs.pointer.booleanValue;
                break;
                
            case kObjectTypeInteger:
                return pointer.integerValue == rhs.pointer.integerValue;
                break;
                
            case kObjectTypeNil:
                return true;
                break;
                
            case kObjectTypeVector:
                if (pointer.vectorPointer->size() == rhs.pointer.vectorPointer->size()) {
                    for (int elementIndex=0; elementIndex < pointer.vectorPointer->size(); ++elementIndex) {
                        if (*(pointer.vectorPointer->at(elementIndex)) != *(rhs.pointer.vectorPointer->at(elementIndex))) {
                            return false;
                        }
                        
                    }
                    return true;
                } else {
                    return false;
                }
                break;
 
            case kObjectTypeSymbol:
            case kObjectTypeString:
                return *pointer.stringValue == *rhs.pointer.stringValue;
                break;
                
            case kObjectTypeCons:
                // recursively evaluate ==, NB lhs equality should return more quickly than rhs equality
                if (*pointer.consValue.left != *rhs.pointer.consValue.left) {
                    return false;
                }
                return *pointer.consValue.right == *rhs.pointer.consValue.right;
                break;
        }
    }
    
    bool Object::operator!=(const Object& rhs) {
        return !operator==(rhs);
    }
    
    Object::Object(bool boolValue) {
        _type = kObjectTypeBoolean;
        pointer.booleanValue = boolValue;
    }
    
    Object::Object(int value) {
        _type = kObjectTypeInteger;
        pointer.integerValue = value;
    }
    
    Object::Object(Object *left, Object *right) {
        _type = kObjectTypeCons;
        pointer.consValue.left = left;
        pointer.consValue.right = right;
    }
    
    Object::Object(std::vector<Object*> objects) {
        _type = kObjectTypeVector;
        pointer.vectorPointer = new std::vector<Object*>(objects);
    }
    
    std::string& Object::stringValue() {
        return *pointer.stringValue;
    }
    
    std::vector<Object*>& Object::vectorValue() {
        return *pointer.vectorPointer;
    }
    
    int& Object::integerValue() {
        return pointer.integerValue;
    }
    
    bool& Object::booleanValue() {
        return pointer.booleanValue;
    }
    
    bool Object::coerceBoolean() {
        switch (_type) {                
            case kObjectTypeInteger:
                return integerValue()!=0;
                break;
                
            case kObjectTypeBoolean:
                return booleanValue();
                break;
                
            case kObjectTypeNil:
                return false;
                break;
                
            default:
                // any object other than nil should be considered true
                return true;
                break;
        }
    }
    
    std::string Object::stringRepresentation(bool expandList) {
        std::stringstream stringBuilder;
        
        switch (_type) {
            case kObjectTypeString:
                stringBuilder << '"' << *pointer.stringValue << '"';
                break;
                
            case kObjectTypeInteger:
                stringBuilder << pointer.integerValue;
                break;
                
            case kObjectTypeVector:
                stringBuilder << "[";
                for (int elementIndex=0; elementIndex<vectorValue().size(); ++elementIndex) {
                    if (elementIndex) {
                        stringBuilder << " ";
                    }
                    stringBuilder << pointer.vectorPointer->at(elementIndex)->stringRepresentation();
                }
                stringBuilder << "]";
                break;
                
            case kObjectTypeBoolean:
                if (pointer.booleanValue) {
                    stringBuilder << "true";
                } else {
                    stringBuilder << "false";
                }
                break;
                
            case kObjectTypeCons: {
                std::vector<Object*> elements;
                
                if (buildList(elements) && expandList) {
                    stringBuilder << "`(";
                    for (int listIndex = 0; listIndex < elements.size(); ++listIndex) {
                        stringBuilder << elements[listIndex]->stringRepresentation(false);
                        
                        if (listIndex < elements.size()-1) {
                            stringBuilder << " ";
                        }
                    }
                    stringBuilder << ")";
                } else {
                    stringBuilder   << "(cons "
                                    << pointer.consValue.left->stringRepresentation()
                                    << " "
                                    << pointer.consValue.right->stringRepresentation()
                                    << ")";
                }
                } break;
                
            case kObjectTypeNil:
                stringBuilder << "nil";
                break;
                
            case kObjectTypeSymbol:
                stringBuilder << *pointer.stringValue;
                break;
        }
        
        return stringBuilder.str();
    }

    bool Object::isList() {
        Object *currentObject = this;
        
        while (currentObject->_type == kObjectTypeCons) {
            if (currentObject->pointer.consValue.right->_type == kObjectTypeCons) {
                // this is a cons with a cons as its right value, continue the list
                currentObject = currentObject->pointer.consValue.right;
            } else if (currentObject->pointer.consValue.right->_type == kObjectTypeNil) {
                // a nil terminator for the list
                return true;
            } else {
                // this isn't a list
                return false;
            }
        }
        
        return false;
    }
    
    bool Object::buildList(std::vector<Object*>& results) {
        Object *currentObject = this;
        
        while (currentObject->_type == kObjectTypeCons) {
            if (currentObject->pointer.consValue.right->_type == kObjectTypeCons) {
                // this is a cons with a cons as its right value, continue the list
                results.push_back(currentObject->pointer.consValue.left);
                currentObject = currentObject->pointer.consValue.right;
            } else if (currentObject->pointer.consValue.right->_type == kObjectTypeNil) {
                // a nil terminator for the list
                results.push_back(currentObject->pointer.consValue.left);
                return true;
            } else {
                // this isn't a list
                return false;
            }
        }
        
        return false;
    }
    
#pragma mark -
#pragma mark TinyClojure
    
    Object* TinyClojure::listObject(std::vector<Object*> list) {
        if (list.size()) {
            if (list.size()==1) {
                // end a list with a nil sentinel
                Object *nilObject = _gc->registerObject(new Object());
                return _gc->registerObject(new Object(list[0], nilObject));
            } else {
                Object *left = list[0];
                list.erase(list.begin());
                return _gc->registerObject(new Object(left, listObject(list)));
            }
        } else {
            // clojure's empty lists seem to be (cons nil nil)
            Object *nilObject = _gc->registerObject(new Object());
            return _gc->registerObject(new Object(nilObject, nilObject));
        }
    }

    
    TinyClojure::TinyClojure() {
        _ioProxy = new IOProxy();
        _gc = new GarbageCollector();
        _newlineSet = std::string("\n\r");
        _baseScope = new InterpreterScope();
        
        for (char excludeChar = 1; excludeChar<32; ++excludeChar) {
            _excludeSet.append(&excludeChar, 1);
        }
        _excludeSet.append("\"()[]{}';` ");
        
        _numberSet = std::string("0123456789");
        
        loadExtensionFunctions();
    }
    
    TinyClojure::~TinyClojure() {
        for (std::map<std::string, ExtensionFunction*>::iterator it = _functionTable.begin(); it != _functionTable.end(); ++it) {
            delete it->second;
        }
        
        delete _baseScope;
        delete _ioProxy;
        delete _gc;
    }
    
    void TinyClojure::addExtensionFunction(ExtensionFunction *function) {
        function->setEvaluator(this);
        function->setGarbageCollector(_gc);
        function->setIOProxy(_ioProxy);
        
        _functionTable[function->functionName()] = function;
    }
    
    void TinyClojure::loadExtensionFunctions() {
        addExtensionFunction(new core::Plus());
        addExtensionFunction(new core::Minus());
        addExtensionFunction(new core::Multiply());
        addExtensionFunction(new core::Divide());
        addExtensionFunction(new core::If());
        addExtensionFunction(new core::Equality());
        addExtensionFunction(new core::Cons());
        addExtensionFunction(new core::List());
        addExtensionFunction(new core::LessThan);
        addExtensionFunction(new core::LessThanOrEqual);
        addExtensionFunction(new core::GreaterThan);
        addExtensionFunction(new core::GreaterThanOrEqual);
        addExtensionFunction(new core::Print());
        addExtensionFunction(new core::Def);
        addExtensionFunction(new core::Do);
        addExtensionFunction(new core::Vector);
        addExtensionFunction(new core:Fn);
    }
    
    void TinyClojure::resetInterpreter() {
        delete _baseScope;
        _baseScope = new InterpreterScope();
    }
    
#pragma mark parser
    
    Object* TinyClojure::parse(std::string startText) {
        ParserState parseState(startText);
        return recursiveParse(parseState);
    }
        
    /**
     * TODO rewrite the entire parser
     *
     * it is a poor translation of Lisping's objc parser,
     * which is a highly tolerant parser, not appropriate for an interpreter
     */
    Object* TinyClojure::recursiveParse(ParserState& parseState) {
        parseState.skipSeparators();
        
        if (parseState.position >= parseState.parserString.length()) {
            // there is nothing here return NULL
            return NULL;
        }
        
        const int startPosition = parseState.position;
        
        char    startChar = parseState.currentChar(),
                peekChar = parseState.peekChar();
        
        enum {
            sexpTypeNormal,
            sexpTypeLambdaShorthand,
            sexpTypeListLiteral,
            sexpTypeHashSet,
        } sexpType = sexpTypeNormal;
        
        enum {
            stringTypeNormal,
            stringTypeRegex,
        } stringType = stringTypeNormal;
        
        if (startChar == '#' && peekChar == '"') {
            stringType = stringTypeRegex;
            startChar = peekChar;
            parseState.position++;
        }
        
        if (startChar == '#' && peekChar == '(') {
            sexpType = sexpTypeLambdaShorthand;
            startChar = peekChar;
            parseState.position++;
        } else if (startChar == '#' && peekChar == '{') {
            sexpType = sexpTypeHashSet;
            startChar = peekChar;
            parseState.position++;
        } else if ((startChar == '\'' || startChar == '`') && (peekChar == '(' || peekChar == '[' || peekChar == '{')) {
            sexpType = sexpTypeListLiteral;
            startChar = peekChar;
            parseState.position++;
        }
        
        if (startChar == '"' || stringType == stringTypeRegex) {
            // start collecting a string
            std::string stringbuf;
            
            bool escapeNextChar = false;

            // ignore the intial string char
            ++parseState.position;
            
            while (parseState.charactersLeft()) {
                const char currentChar = parseState.currentChar();
                
                /// TODO check for escapes
                stringbuf.append(&currentChar, 1);
                ++parseState.position;
                
                if (escapeNextChar) {
                    escapeNextChar = false;
                } else {
                    if (currentChar == '\\') {
                        escapeNextChar = true;
                    } else if (currentChar=='"' && stringbuf.length()>1) {
                        // TODO do something in the case of regexes
                        if (stringType == stringTypeRegex) {
                            std::cerr << "Not dealing with regexes" << std::endl;
                        }
                        
                        // remove the " that has been collected from the end of the string
                        stringbuf = stringbuf.substr(0, stringbuf.length()-1);
                        
                        // end of the string
                        Object *element = new Object(stringbuf);
                        _gc->registerObject(element);
                        return element;
                    }
                }
            }
            
            Error error(parseState, std::string("Ran out of characters when parsing a string"));
            throw error;
        } else if (startChar == '(' || sexpType == sexpTypeHashSet) {
            char closeChar = ')';
            if (sexpType == sexpTypeHashSet) closeChar = '}';
            
            /*
             * start an S-Expression
             *
             * We are looking for
             * * Open parenthesis (found)
             * * terms (recurse this function)
             * * Close parenthesis
             */
            std::vector<Object*> elements;
            
            // advance from the parenthesis
            ++parseState.position;
            
            while (true) {
                // move on
                parseState.skipSeparators();
                
                if (!parseState.charactersLeft()) {
                    // nothing left, return the s expression so far
                    break;
                }
                
                if (parseState.currentChar()==closeChar) {
                    // advance past the ) and end the S expression
                    ++parseState.position;
                    
                    break;
                }
                
                Object *element = recursiveParse(parseState);
                if (element) {
                    elements.push_back(element);
                } else {
                    // nothing left, return the s expression so far
                    Error error(parseState, "Ran out of characters when building an S Expression");
                    throw error;
                    break;
                }
            }
            
            switch (sexpType) {
                case sexpTypeNormal:
                    return listObject(elements);
                    break;
                    
                case sexpTypeListLiteral:
                    elements.insert(elements.begin(), _gc->registerObject(new Object(std::string("list", true))));
                    return listObject(elements);
                    break;
                    
                case sexpTypeLambdaShorthand:
                    _ioProxy->writeErr("lambda shorthand unimplemented");
                    return NULL;
                    break;
                    
                case sexpTypeHashSet:
                    elements.insert(elements.begin(), _gc->registerObject(new Object(std::string("hash-set", true))));
                    return listObject(elements);
                    break;
            }
            
            return NULL;
        } else if (startChar == '[') {
            /*
             * start a Clojure vector
             *
             * We are looking for
             * * Open parenthesis (found)
             * * terms (recurse this function)
             * * Close parenthesis
             */
            std::vector<Object*> elements;
            
            // advance from the parenthesis
            ++parseState.position;
            
            while (true) {
                // move on
                parseState.skipSeparators();
                
                if (!parseState.charactersLeft()) {
                    Error error(parseState, "Ran out of characters when building a vector");
                    throw error;
                }
                
                if (parseState.currentChar()==']') {
                    // advance past the ] and end the vector
                    ++parseState.position;

                    // insert the vector identifier at the beginning
                    elements.insert(elements.begin(), _gc->registerObject(new Object("vector", true)));
                    
                    return listObject(elements);
                }
                
                Object *element = recursiveParse(parseState);
                if (element) {
                    elements.push_back(element);
                } else {
                    Error error(parseState, "Ran out of characters when building a vector");
                    throw error;
                }
            }
        } else if (startChar == '{') {
            /*
             * start a Clojure map
             *
             * We are looking for
             * * Open parenthesis (found)
             * * terms (recurse this function)
             * * Close parenthesis
             */
            
            std::vector<Object*> elements;
            
            // advance beyond the parenthesis
            ++parseState.position;
            
            while (true) {
                parseState.skipSeparators();
                
                if (!parseState.charactersLeft()) {
                    Error error(parseState, "Ran out of characters when building a map");
                    throw error;
                }
                
                if (parseState.currentChar()=='}') {
                    // advance past the ] and end the vector
                    ++parseState.position;
                    
                    return listObject(elements);
                }
                
                Object *element = recursiveParse(parseState);
                if (element) {
                    elements.push_back(element);
                } else {
                    Error error(parseState, "Ran out of characters when building a map");
                    throw error;
                }
            }
        } else if (startChar == ';') {
            /// start a comment, run until the end of the line
            std::string comment;
            
            while (parseState.charactersLeft()) {
                const char currentChar = parseState.currentChar();
                
                bool commentChar = false;
                
                if (_newlineSet.find(currentChar) == _newlineSet.npos) {
                    comment.append(&currentChar, 1);
                    commentChar = true;
                    ++parseState.position;
                }
                
                if (!commentChar || !parseState.charactersLeft()) {
                    // end of the comment, ignore it
                    return recursiveParse(parseState);
                }
            }
        } else if (startChar == '`' || startChar == '\'') {
            // start collecting a symbol
            // TODO no real difference between symbols, identifiers and numbers
            std::string symbol;
            
            while (parseState.charactersLeft()) {
                const char  currentChar = parseState.currentChar(),
                            peekChar = parseState.peekChar(),
                            peekPeekChar = parseState.peekPeekChar();
                
                bool identifierChar = false;
                
                if(_excludeSet.find(currentChar) == _excludeSet.npos
                    || (symbol.length() == 0)) {
                    symbol.append(&currentChar, 1);
                    identifierChar = false;
                    ++parseState.position;
                }
                
                if (!identifierChar
                    || !parseState.charactersLeft()
                    || (peekChar=='#' && peekPeekChar=='"')) {
                    // this is a literal symbol xxx, translate to (quote xxx) and push that
                    std::vector<Object *> els;
                    els.push_back(_gc->registerObject(new Object("quote", true)));
                    els.push_back(_gc->registerObject(new Object(symbol, true)));
                    return listObject(els);
                }
            }
        } else if (_excludeSet.find(startChar) == _excludeSet.npos) {
            // start an identifier
            std::string identifier;
            
            while (parseState.charactersLeft()) {
                const char  currentChar = parseState.currentChar(),
                            peekChar = parseState.peekChar(),
                            peekPeekChar = parseState.peekPeekChar();
                
                bool identifierChar = false;
                
                if (_excludeSet.find(currentChar) == _excludeSet.npos
                    || ((startChar=='#') && (currentChar==';') && (parseState.position == startPosition+1))) {    // case to deal with inline comments
                    identifier.append(&currentChar, 1);
                    identifierChar = true;
                    ++parseState.position;
                }
                
                if (!identifierChar
                    || !parseState.charactersLeft()
                    || (peekChar=='#' && peekPeekChar=='"')) {
                    if (identifier.find("#;")==0) {
                        // inline comment element
                        return recursiveParse(parseState);
                    } else {
                        // check for known symbol names
                        if (identifier == "true") {
                            return _gc->registerObject(new Object(true));
                        } else if (identifier == "false") {
                            return _gc->registerObject(new Object(false));
                        } else if (identifier == "nil") {
                            return _gc->registerObject(new Object());
                        }
                        
                        // TODO test for number properly (ie decimals)
                        bool isInteger = true;
                        for (int identifierIndex = 0; identifierIndex < identifier.size(); ++identifierIndex) {
                            if (_numberSet.find(identifier[identifierIndex]) == _numberSet.npos) {
                                isInteger = false;
                                break;
                            }
                        }
                        
                        if (isInteger) {
                            return _gc->registerObject(new Object(atoi(identifier.c_str())));
                        } else {
                            return _gc->registerObject(new Object(identifier, true));
                        }
                    }
                }            
            }
        }
                
        return NULL;
    }
    
#pragma mark evaluator
    
    Object* TinyClojure::recursiveEval(InterpreterScope *interpreterState, Object *code) {
        switch (code->type()) {
            case Object::kObjectTypeNil:
            case Object::kObjectTypeInteger:
            case Object::kObjectTypeString:
            case Object::kObjectTypeBoolean:
                return code;
                break;
                
            case Object::kObjectTypeVector: {
                std::vector<Object*> elements;
                
                for (int elementIndex=0; elementIndex<code->vectorValue().size(); ++elementIndex) {
                    elements[elementIndex] = recursiveEval(interpreterState, code->vectorValue()[elementIndex]);
                }
                
                return _gc->registerObject(new Object(elements));
                } break;
        
            case Object::kObjectTypeSymbol: {
                Object *symbolValue = interpreterState->lookupSymbol(code->stringValue());
                if (symbolValue) {
                    return symbolValue;
                } else {
                    std::stringstream stringBuilder;
                    stringBuilder << "Do not understand symbol " << code->stringValue();
                    throw Error(stringBuilder.str());
                }
            } break;
        
            case Object::kObjectTypeCons:
                if (code->isList()) {
                    std::vector<Object*> elements;
                    code->buildList(elements);
                    Object *identifierObject = elements[0];
                    elements.erase(elements.begin());   // elements now contains the arguments to the function
                    
                    if (identifierObject->type()==Object::kObjectTypeSymbol) {
                        std::map<std::string, ExtensionFunction*>::iterator it = _functionTable.find(identifierObject->stringValue());
                        if (it == _functionTable.end()) {
                            std::stringstream stringBuilder;
                            stringBuilder   << "Do not understand symbol "
                                            << identifierObject->stringValue()
                                            << " TODO, potentially it is user defined";
                            throw Error(stringBuilder.str());
                        } else {
                            ExtensionFunction *function = it->second;
                            
                            int minArgs = function->minimumNumberOfArguments(),
                                maxArgs = function->maximumNumberOfArguments();
                            
                            if (minArgs>=0) {
                                if (elements.size() < minArgs) {
                                    std::stringstream stringBuilder;
                                    stringBuilder   << "Function "
                                                    << function->functionName()
                                                    << " requires at least "
                                                    << minArgs
                                                    << " arguments"
                                                    << std::endl;
                                    
                                    throw Error(stringBuilder.str());
                                }
                            }
                            
                            if (maxArgs>=0) {
                                if (elements.size() > maxArgs) {
                                    std::stringstream stringBuilder;
                                    stringBuilder   << "Function "
                                                    << function->functionName()
                                                    << " requires no more than "
                                                    << maxArgs
                                                    << " arguments"
                                                    << std::endl;
                                    
                                    throw Error(stringBuilder.str());
                                }
                            }
                            
                            Object *result = function->execute(elements, interpreterState);
                            if (result==NULL) {
                                result = _gc->registerObject(new Object());
                            }
                            
                            return result;
                        }
                    } else {
                        throw Error("An executable S Expression should begin with an identifier");
                    }
                } else {
                    // I could be wrong, but I don't think this case makes any sense, most likely we got here cos of a flaw in buildList
                    throw Error("An executable S Expression was not understood");
                }
                break;
        }
        
        return NULL;        
    }
    
    Object* TinyClojure::eval(Object* code) {
        Object *ret = recursiveEval(_baseScope, code);
        
        if (ret==NULL) {
            ret = _gc->registerObject(new Object());
        }
        
        return ret;
    }

#pragma mark -
#pragma mark Garbage Collector
    
    Object* GarbageCollector::registerObject(Object* object) {
        _objects.insert(object);
        return object;
    }
    
    GarbageCollector::GarbageCollector() {
    }
    
    GarbageCollector::~GarbageCollector() {
        for (std::set<Object*>::iterator it = _objects.begin(); it != _objects.end(); ++it)
            delete *it;
    }
    
    Object* GarbageCollector::retainRootObject(Object *object) {
        return object;
    }
    
    Object* GarbageCollector::releaseRootObject(Object *object) {
        return object;
    }
    
    void GarbageCollector::collectGarbage() {
    }

#pragma mark -
#pragma mark ParserState

    int ParserState::skipCharactersInString(std::string skipSet) {
        int numberOfSkippedCharacters = 0;
        
        while (charactersLeft()) {
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

    char ParserState::peekChar() {
        if (position < parserString.length()-1) {
            return parserString[position+1];
        }
        
        return 0;
    }

    char ParserState::peekPeekChar() {
        if (position < parserString.length()-2) {
            return parserString[position+2];
        }
        
        return 0;
    }
}