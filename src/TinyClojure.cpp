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
#pragma mark Number
    
    Number::Number(double val) {
        setFloating(val);
    }
    
    Number::Number(int val) {
        setInteger(val);
    }
    
    Number::Number() {
        setInteger(0);
    }
    
    double Number::floatingValue() const {
        switch (_mode) {
            case kNumberModeFloating:
                return _value.floating;
                break;
                
            case kNumberModeInteger:
                return (double)_value.integer;
                break;
        }
    }
    
    Number Number::floatingVersion() const {
        return Number(floatingValue());
    }
    
    int Number::integerValue() const {
        switch (_mode) {
            case kNumberModeInteger:
                return _value.integer;
                break;
                
            case kNumberModeFloating:
                return (int)std::round(_value.floating);
                break;
        }
    }
    
    Number Number::integerVersion() const {
        return Number(floatingValue());
    }
    
    Number Number::operator+(const Number& rhs) const {
        Number ilhs,irhs;
        equivalentModes(*this, rhs, ilhs, irhs);
        
        switch (ilhs._mode) {
            case kNumberModeFloating:
                return Number(ilhs._value.floating + irhs._value.floating);
                break;
                
            case kNumberModeInteger:
                return Number(ilhs._value.integer + irhs._value.integer);
                break;
        }
    }
    
    Number Number::operator*(const Number& rhs) const {
        Number ilhs,irhs;
        equivalentModes(*this, rhs, ilhs, irhs);
        
        switch (ilhs._mode) {
            case kNumberModeFloating:
                return Number(ilhs._value.floating * irhs._value.floating);
                break;
                
            case kNumberModeInteger:
                return Number(ilhs._value.integer * irhs._value.integer);
                break;
        }
    }
    
    Number Number::operator/(const Number& rhs) const {
        return Number(floatingValue() / rhs.floatingValue());
    }
    
    Number Number::operator-(const Number& rhs) const {
        Number ilhs,irhs;
        equivalentModes(*this, rhs, ilhs, irhs);
        
        switch (ilhs._mode) {
            case kNumberModeFloating:
                return Number(ilhs._value.floating - irhs._value.floating);
                break;
                
            case kNumberModeInteger:
                return Number(ilhs._value.integer - irhs._value.integer);
                break;
        }
    }

    void Number::equivalentModes(const Number& originalLHS, const Number& originalRHS, Number& newLHS, Number& newRHS) {
        if (originalLHS._mode==originalRHS._mode) {
            newLHS=originalLHS;
            newRHS=originalRHS;
        } else {
            newLHS = originalLHS.floatingVersion();
            newRHS = originalRHS.floatingVersion();
        }
    }
    
    void Number::setFloating(double val) {
        _value.floating = val;
        _mode = kNumberModeFloating;
    }
    
    void Number::setInteger(int val) {
        _value.integer = val;
        _mode = kNumberModeInteger;
    }
    
    std::string Number::stringRepresentation() const {
        std::stringstream stringBuilder;
        
        switch (_mode) {
            case kNumberModeFloating:
                stringBuilder << _value.floating;
                break;
                
            case kNumberModeInteger:
                stringBuilder << _value.integer;
                break;
        }
        
        return stringBuilder.str();
    }

    bool Number::operator==(const Number& rhs) const {
        Number ilhs,irhs;
        equivalentModes(*this, rhs, ilhs, irhs);
        
        switch (ilhs._mode) {
            case kNumberModeFloating:
                return ilhs._value.floating == irhs._value.floating;
                break;
                
            case kNumberModeInteger:
                return ilhs._value.integer == irhs._value.integer;
                break;
        }
    }
    
    bool Number::operator<(const Number& rhs) const {
        Number ilhs,irhs;
        equivalentModes(*this, rhs, ilhs, irhs);
        
        switch (ilhs._mode) {
            case kNumberModeFloating:
                return ilhs._value.floating < irhs._value.floating;
                break;
                
            case kNumberModeInteger:
                return ilhs._value.integer < irhs._value.integer;
                break;
        }
    }
    
    bool Number::operator>(const Number& rhs) const {
        Number ilhs,irhs;
        equivalentModes(*this, rhs, ilhs, irhs);
        
        switch (ilhs._mode) {
            case kNumberModeFloating:
                return ilhs._value.floating > irhs._value.floating;
                break;
                
            case kNumberModeInteger:
                return ilhs._value.integer > irhs._value.integer;
                break;
        }
    }
    
    bool Number::operator<=(const Number& rhs) const {
        Number ilhs,irhs;
        equivalentModes(*this, rhs, ilhs, irhs);
        
        switch (ilhs._mode) {
            case kNumberModeFloating:
                return ilhs._value.floating <= irhs._value.floating;
                break;
                
            case kNumberModeInteger:
                return ilhs._value.integer <= irhs._value.integer;
                break;
        }
    }
    
    bool Number::operator>=(const Number& rhs) const {
        Number ilhs,irhs;
        equivalentModes(*this, rhs, ilhs, irhs);
        
        switch (ilhs._mode) {
            case kNumberModeFloating:
                return ilhs._value.floating >= irhs._value.floating;
                break;
                
            case kNumberModeInteger:
                return ilhs._value.integer >= irhs._value.integer;
                break;
        }
    }
    
    bool Number::operator!=(const Number& rhs) const {
        return !operator==(rhs);
    }
    
#pragma mark - ExtensionFunction
    
    void ExtensionFunction::garbageCollector(GarbageCollector *gc) {
        _gc = gc;
    }
    void ExtensionFunction::evaluator(TinyClojure *evaluator) {
        _evaluator = evaluator;
    }
    
    
    void ExtensionFunction::setIOProxy(IOProxy *ioProxy) {
        _ioProxy = ioProxy;
    }
    
    bool ExtensionFunction::validateArgumentTypes(std::vector<Object::ObjectType>& typeArray) {
        if (_typeArray.size()==0)
            return true;

        if (_typeArray.size() != typeArray.size())
            return false;
        
        for (int typeIndex = 0; typeIndex < _typeArray.size(); ++typeIndex) {
            if (typeArray[typeIndex] != _typeArray[typeIndex]) {
                return false;
            }
        }
        
        return true;
    }
    
    int ExtensionFunction::requiredNumberOfArguments() {
        if (_typeArray.size()) {
            return (int)_typeArray.size();
        }
        
        return -1;
    }

    void ExtensionFunction::setup() {
        _typeArray.clear();
        fillTypeArray();
    }
    
#pragma mark -
#pragma mark Standard Library
    
    namespace core {
        /// function for adding a list of numbers
        class Arithmetic : public ExtensionFunction {
            virtual Number numberOperation(Number lhs, Number rhs) = 0;
            
            int minimumNumberOfArguments() {
                return 1;
            }
            
            Object* execute(ObjectList arguments, InterpreterScope* interpreterState) {
                bool first = true;
                
                Number current;

                for (int argumentIndex = 0; argumentIndex < arguments.size(); ++argumentIndex) {
                    Object *evaluatedArgument = arguments[argumentIndex];
                    if (evaluatedArgument->type() != Object::kObjectTypeNumber) {
                        throw Error("Arithmetic functions require all arguments to be numbers");
                    }
                    
                    if (first) {
                        current = evaluatedArgument->numberValue();
                        first = false;
                    } else {
                        current = numberOperation(current, evaluatedArgument->numberValue());
                    }
                }
                
                return _gc->registerObject(new Object(current));
            }
        };
        
        class Plus : public Arithmetic {
            std::string functionName() {
                return std::string("+");
            }
            
            virtual Number numberOperation(Number lhs, Number rhs) {
                return lhs + rhs;
            }
        };
        
        class Minus : public Arithmetic {
            std::string functionName() {
                return "-";
            }
            
            virtual Number numberOperation(Number lhs, Number rhs) {
                return lhs - rhs;
            }
        };
        
        class Multiply : public Arithmetic {
            std::string functionName() {
                return "*";
            }
            
            virtual Number numberOperation(Number lhs, Number rhs) {
                return lhs * rhs;
            }
        };
        
        class Divide  : public Arithmetic {
            std::string functionName() {
                return "/";
            }
            
            virtual Number numberOperation(Number lhs, Number rhs) {
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
            
            bool preEvaluateArguments() {
                return false;
            }
            
            Object *execute(ObjectList arguments, InterpreterScope* interpreterState) {
                Object  *condition = arguments[0],
                        *trueBranch = arguments[1],
                        *falseBranch = NULL;
                
                if (arguments.size()==3) {
                    falseBranch = arguments[2];
                } else {
                    falseBranch = _gc->registerObject(new Object());
                }
                
                Object *evaluatedCondition = _evaluator->scopedEval(interpreterState, condition);
                if (evaluatedCondition->coerceBoolean()) {
                    return _evaluator->scopedEval(interpreterState, trueBranch);
                } else {
                    return _evaluator->scopedEval(interpreterState, falseBranch);
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
            
            bool preEvaluateArguments() {
                return false;
            }
            
            Object *execute(ObjectList arguments, InterpreterScope* interpreterState) {
                Object *lhs = _evaluator->scopedEval(interpreterState, arguments[0]);
                
                for (int argumentIndex = 1; argumentIndex < arguments.size(); ++argumentIndex) {
                    Object *rhs = _evaluator->scopedEval(interpreterState, arguments[argumentIndex]);
                    
                    if (*lhs!=*rhs) {
                        return _gc->registerObject(new Object(false));
                    }
                }
                
                return _gc->registerObject(new Object(true));
            }
        };

        class NotEqual : public ExtensionFunction {
            std::string functionName() {
                return "not=";
            }
            
            int minimumNumberOfArguments() {
                return 1;
            }
            
            bool preEvaluateArguments() {
                return false;
            }
            
            Object *execute(ObjectList arguments, InterpreterScope* interpreterState) {
                Object *lhs = _evaluator->scopedEval(interpreterState, arguments[0]);
                
                for (int argumentIndex = 1; argumentIndex < arguments.size(); ++argumentIndex) {
                    Object *rhs = _evaluator->scopedEval(interpreterState, arguments[argumentIndex]);
                    
                    if (*lhs==*rhs) {
                        return _gc->registerObject(new Object(false));
                    }
                }
                
                return _gc->registerObject(new Object(true));
            }
        };
        
        class NumericInequality : public ExtensionFunction {
            Object *execute(ObjectList arguments, InterpreterScope* interpreterState) {
                ObjectList evaluatedArguments;
                for (int argumentIndex=0; argumentIndex<arguments.size(); ++argumentIndex) {
                    evaluatedArguments.push_back(_evaluator->scopedEval(interpreterState, arguments[argumentIndex]));
                    
                    if (evaluatedArguments.back()->type() != Object::kObjectTypeNumber) {
                        std::stringstream stringBuilder;
                        
                        stringBuilder   << "Arguments to "
                                        << functionName()
                                        << " must all be numeric";
                        
                        throw Error(stringBuilder.str());
                    }
                }

                Object *lhs = _evaluator->scopedEval(interpreterState, arguments[0]);
                
                for (int argumentIndex = 1; argumentIndex < arguments.size(); ++argumentIndex) {
                    Object *rhs = _evaluator->scopedEval(interpreterState, arguments[argumentIndex]);
                    
                    if (!comparison(lhs->numberValue(), rhs->numberValue())) {
                        return _gc->registerObject(new Object(false));
                    }
                }
                
                return _gc->registerObject(new Object(true));
            }
            
            bool preEvaluateArguments() {
                return false;
            }
            
        protected:
            virtual bool comparison(Number lhs, Number rhs) = 0;
        };
        
        class LessThan : public NumericInequality {
            std::string functionName() {
                return "<";
            }
            
            bool comparison(Number lhs, Number rhs) {
                return lhs < rhs;
            }
        };

        class GreaterThan : public NumericInequality {
            std::string functionName() {
                return ">";
            }
            
            bool comparison(Number lhs, Number rhs) {
                return lhs > rhs;
            }
        };

        class LessThanOrEqual : public NumericInequality {
            std::string functionName() {
                return "<=";
            }
            
            bool comparison(Number lhs, Number rhs) {
                return lhs <= rhs;
            }
        };

        class GreaterThanOrEqual : public NumericInequality {
            std::string functionName() {
                return ">=";
            }
            
            bool comparison(Number lhs, Number rhs) {
                return lhs >= rhs;
            }
        };
        
        class Vector : public ExtensionFunction {
            std::string functionName() {
                return "vector";
            }
            
            Object* execute(ObjectList arguments, InterpreterScope *interpreterState) {
                return _gc->registerObject(new Object(arguments));
            }
        };
        
        class List : public ExtensionFunction {
            std::string functionName() {
                return "list";
            }
            
            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {
                Object *null = _gc->registerObject(new Object());

                if (arguments.size()==0) {
                    return _gc->registerObject(new Object(null,null));
                } else if (arguments.size()==1) {
                    return _gc->registerObject(new Object(arguments[0],null));
                } else {
                    Object *rhs = null;
                    
                    for (long argumentIndex = arguments.size()-1; argumentIndex >= 0; --argumentIndex) {
                        Object *lhs = arguments[argumentIndex];
                        
                        rhs = _gc->registerObject(new Object(lhs, rhs));
                    }
                    
                    return rhs;
                }
            }
        };
                
        class Cons : public ExtensionFunction {
            std::string functionName() {
                return "cons";
            }
            
            int requiredNumberOfArguments() {
                return 2;
            }
            
            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {
                return _gc->registerObject(new Object(arguments[0], arguments[1]));
            }
        };
        
        class Print : public ExtensionFunction {
            std::string functionName() {
                return "print";
            }
            
            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {
                for (int argumentIndex=0; argumentIndex<arguments.size(); ++argumentIndex) {
                    std::string thisArgument = arguments[argumentIndex]->stringRepresentation();
                    
                    _ioProxy->writeOut(thisArgument);
                    _ioProxy->writeOut("\n");
                }
                
                return _gc->registerObject(new Object());
            }
        };
        
        class Def : public ExtensionFunction {
            std::string functionName() {
                return "def";
            }
            
            int requiredNumberOfArguments() {
                return 2;
            }
            
            bool preEvaluateArguments() {
                return false;
            }
            
            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {
                Object  *symbol = arguments[0],
                        *value = _evaluator->scopedEval(interpreterState, arguments[1]);
                
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
            
            bool preEvaluateArguments() {
                return false;
            }
            
            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {
                InterpreterScope aScope(interpreterState);
                
                Object  *retValue = _gc->registerObject(new Object());
                
                for (int argumentIndex = 0; argumentIndex < arguments.size(); ++argumentIndex) {
                    retValue = _evaluator->unscopedEval(&aScope, arguments[argumentIndex]);
                }
                
                return retValue;
            }
        };
        
        class Closure : public ExtensionFunction {
        public:
            bool preEvaluateArguments() {
                return false;
            }
            
            Object* captureState(Object *object, InterpreterScope *interpreterState) {
                switch (object->type()) {
                    case Object::kObjectTypeBoolean:
                    case Object::kObjectTypeNil:
                    case Object::kObjectTypeNumber:
                    case Object::kObjectTypeString:
                    case Object::kObjectTypeBuiltinFunction:
                    case Object::kObjectTypeClosure:
                        return object;
                        break;
                        
                    case Object::kObjectTypeCons: {
                        Object  *left = captureState(object->consValueLeft(), interpreterState),
                        *right = captureState(object->consValueRight(), interpreterState);
                        
                        return _gc->registerObject(new Object(left, right));
                    } break;
                        
                    case Object::kObjectTypeVector: {
                        std::vector<Object *> newVector;
                        
                        for (int vectorIndex = 0; vectorIndex < object->vectorValue().size(); ++vectorIndex) {
                            newVector.push_back(captureState(object->vectorValue()[vectorIndex], interpreterState));
                        }
                        
                        return _gc->registerObject(new Object(newVector));
                    } break;
                        
                    case Object::kObjectTypeSymbol: {
                        Object *lookedUpValue = interpreterState->lookupSymbol(object->stringValue());
                        
                        if (lookedUpValue) {
                            return lookedUpValue;
                        } else {
                            return object;
                        }
                    } break;
                }
            }
            
            void constructArgumentList(Object *arglist, ObjectList& argumentSymbols) {
                // construct the argument list
                ObjectList parameterSymbols;
                bool validArgumentList = false;
                if (arglist->buildList(parameterSymbols)) {
                    if (parameterSymbols.size()) {
                        if (parameterSymbols[0]->type()==Object::kObjectTypeSymbol) {
                            if (parameterSymbols[0]->stringValue()=="vector") {
                                validArgumentList = true;
                            }
                        } else if (parameterSymbols[0]->type()==Object::kObjectTypeBuiltinFunction) {
                            if (parameterSymbols[0]->functionValueExtensionFunction()->functionName() == "vector") {
                                validArgumentList = true;
                            }
                        }
                        
                    }
                }
                
                if (validArgumentList) {
                    // grab each individual argument following the "vector symbol"
                    for (int argumentIndex=1; argumentIndex < parameterSymbols.size(); ++argumentIndex) {
                        if (parameterSymbols[argumentIndex]->type() == Object::kObjectTypeSymbol) {
                            argumentSymbols.push_back(parameterSymbols[argumentIndex]);
                        } else {
                            validArgumentList = false;
                        }
                    }
                } else {
                    throw Error("Could not build argument list");
                }
            }
        };
        
        class Defn : public Closure {
        public:
            std::string functionName() {
                return "defn";
            }
            
            int minimumNumberOfArguments() {
                return 3;
            }
            
            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {
                // symbol
                Object  *symbol = arguments[0],
                        *arglist = arguments[1];
                
                if (symbol->type()!=Object::kObjectTypeSymbol) {
                    throw Error("first argument to defn must be a symbol");
                }
                
                // construct an argument list
                ObjectList argumentSymbols;
                constructArgumentList(arglist, argumentSymbols);
                
                // remove the initial argument and symbol, just leaving the function body
                arguments.erase(arguments.begin());
                arguments.erase(arguments.begin());
                
                // capture the arguments
                ObjectList capturedArguments;
                capturedArguments.push_back(_gc->registerObject(new Object("do", true)));
                for (int argumentIndex = 0; argumentIndex < arguments.size(); ++argumentIndex) {
                    capturedArguments.push_back(captureState(arguments[argumentIndex], interpreterState));
                }
                
                Object *lambda = _gc->registerObject(new Object(_evaluator->listObject(capturedArguments), argumentSymbols));
                
                interpreterState->setSymbolInScope(symbol->stringValue(), lambda);
                
                return _gc->registerObject(new Object());
            }
        };
        
        class Fn : public Closure {
            std::string functionName() {
                return "fn";
            }
            
            int minimumNumberOfArguments() {
                return 2;
            }
                        
            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {
                // construct an argument list
                ObjectList argumentSymbols;
                constructArgumentList(arguments[0], argumentSymbols);
                
                // remove the initial argument, just leaving the function body
                arguments.erase(arguments.begin());

                // capture the arguments
                ObjectList capturedArguments;
                capturedArguments.push_back(_gc->registerObject(new Object("do", true)));
                for (int argumentIndex = 0; argumentIndex < arguments.size(); ++argumentIndex) {
                    capturedArguments.push_back(captureState(arguments[argumentIndex], interpreterState));
                }
                
                return _gc->registerObject(new Object(_evaluator->listObject(capturedArguments), argumentSymbols));
            }
        };
        
        class ConsFunction : public ExtensionFunction {
            int requiredNumberOfArguments() {
                return 1;
            }
            
            virtual Object* operation(Object *consObject) = 0;
            
            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {
                if (arguments[0]->type() != Object::kObjectTypeCons) {
                    std::stringstream stringBuilder;
                    
                    stringBuilder   << "arguments to "
                                    << functionName()
                                    << "must be of cons type";
                    
                    throw Error(stringBuilder.str());
                }
                
                return operation(arguments[0]);
            }
        };
        
        class First : public ConsFunction {
            std::string functionName() {
                return "first";
            }
            
            Object *operation(Object *consObject) {
                return consObject->consValueLeft();
            }
        };

        class Rest : public ConsFunction {
            std::string functionName() {
                return "rest";
            }
            
            Object *operation(Object *consObject) {
                return consObject->consValueRight();
            }
        };
        
        class ReadString : public ExtensionFunction {
            std::string functionName() {
                return "read-string";
            }
            
            void fillTypeArray() {
                _typeArray.push_back(Object::kObjectTypeString);
            }
            
            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {
                return _evaluator->parse(arguments[0]->stringValue());
            }
        };
        
        class Eval : public ExtensionFunction {
            std::string functionName() {
                return "eval";
            }
            
            int requiredNumberOfArguments() {
                return 1;
            }
            
            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {
                return _evaluator->scopedEval(interpreterState, arguments[0]);
            }
        };
        
        class ReadLine : public ExtensionFunction {
            std::string functionName() {
                return "read-line";
            }
            
            int requiredNumberOfArguments() {
                return 0;
            }
            
            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {                
                return _gc->registerObject(new Object(_ioProxy->readLine()));
            }
        };
        
        class Cond : public ExtensionFunction {
            std::string functionName() {
                return "cond";
            }
            
            bool preEvaluateArguments() {
                return false;
            }
            
            Object* execute(ObjectList arguments, InterpreterScope* interpreterState) {
                if (arguments.size() % 2 != 0) {
                    throw Error("The cond form requires an even number of arguemnts");
                }
                
                for (int firstArgumentIndex = 0; firstArgumentIndex < arguments.size(); firstArgumentIndex += 2) {
                    Object  *firstArgument = arguments[firstArgumentIndex],
                            *secondArgument = arguments[firstArgumentIndex+1]; // we known arguments has even length
                    
                    if (_evaluator->scopedEval(interpreterState, firstArgument)->coerceBoolean()) {
                        return _evaluator->scopedEval(interpreterState, secondArgument);
                    }
                }
                
                return _gc->registerObject(new Object());
            };
        };
        
        class Let : public ExtensionFunction {
            std::string functionName() {
                return "let";
            }
            
            bool preEvaluateArguments() {
                return false;
            }
            
            int minimumNumberOfArguments() {
                return 1;
            }
            
            Object* execute(ObjectList arguments, InterpreterScope *interpreterState) {
                InterpreterScope letScope(interpreterState);
                
                ObjectList bindings;
                
                // initial checks
                if (!arguments[0]->buildList(bindings)) {
                    throw Error("First argument to let statement must be a vector of bindings");
                }
                
                if (bindings[0]->type() != Object::kObjectTypeSymbol) {
                    throw Error("First argument to let statement must be a vector of bindings");
                }
                
                if (bindings[0]->stringValue() != std::string("vector")) {
                    throw Error("First argument to let statement must be a vector of bindings");
                }
                
                if (!bindings.size()) {
                    throw Error("First argument to let statement must be a vector of bindings");
                }
                
                if (bindings.size() % 2 != 1) {
                    throw Error("First argument of let statement must consist of variables and values");
                }
                
                // set bindings
                for (int bindingIndex = 1; bindingIndex < bindings.size(); bindingIndex += 2) {
                    Object  *bindingSymbol = bindings[bindingIndex],
                            *bindingValue = bindings[bindingIndex+1],
                            *evaluatedBindingValue = _evaluator->scopedEval(&letScope, bindingValue);
                    
                    if (bindingSymbol->type() != Object::kObjectTypeSymbol) {
                        throw Error("Let bindings should consist of symbol/value pairs");
                    }
                    
                    letScope.setSymbolInScope(bindingSymbol->stringValue(), evaluatedBindingValue);
                }
                
                // now evaluate the arguments in turn
                Object  *retValue = _gc->registerObject(new Object());
                
                for (int argumentIndex = 1; argumentIndex < arguments.size(); ++argumentIndex) {
                    retValue = _evaluator->unscopedEval(&letScope, arguments[argumentIndex]);
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
    
    void InterpreterScope::setSymbolInScope(std::string symbolName, Object *functionValue) {
        _symbolTable[symbolName] = functionValue;
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
    
    Object::Object(Object *code, ObjectList arguments) {
        _type = kObjectTypeClosure;
        pointer.functionValue.objectPointer = code;
        pointer.functionValue.argumentSymbols = new ObjectList(arguments);
    }
    
    Object::Object(ExtensionFunction *function) {
        _type = kObjectTypeBuiltinFunction;
        pointer.builtinFunctionValue.extensionFunctionPointer = function;
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
            
            case kObjectTypeClosure:
                // leave the Objects to the gc
                delete pointer.functionValue.argumentSymbols;
                break;

            case kObjectTypeNumber:
                delete pointer.numberPointer;
                break;
                
            case kObjectTypeCons:
                // it isn't our business deleting "unused" objects, that is for the GC
            case kObjectTypeBuiltinFunction:
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
                
            case kObjectTypeNumber:
                return *pointer.numberPointer == *rhs.pointer.numberPointer;
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
                
            case kObjectTypeBuiltinFunction:
                return pointer.builtinFunctionValue.extensionFunctionPointer->functionName() == rhs.pointer.builtinFunctionValue.extensionFunctionPointer->functionName();
                break;
                
            case kObjectTypeClosure:
                return *pointer.functionValue.objectPointer == *rhs.pointer.functionValue.objectPointer;
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
    
    Object* Object::functionValueCode() {
        return pointer.functionValue.objectPointer;
    }

    ExtensionFunction* Object::functionValueExtensionFunction() {
        return pointer.builtinFunctionValue.extensionFunctionPointer;
    }
    
    ObjectList Object::functionValueParameters() {
        return *pointer.functionValue.argumentSymbols;
    }
    
    Object* Object::consValueLeft() {
        return pointer.consValue.left;
    }
    
    Object* Object::consValueRight() {
        return pointer.consValue.right;
    }
    
    Object::Object(Number numberValue) {
        _type = kObjectTypeNumber;
        pointer.numberPointer = new Number(numberValue);
    }
    
    Object::Object(bool boolValue) {
        _type = kObjectTypeBoolean;
        pointer.booleanValue = boolValue;
    }
    
    Object::Object(int val) {
        _type = kObjectTypeNumber;
        pointer.numberPointer = new Number(val);
    }

    Object::Object(double val) {
        _type = kObjectTypeNumber;
        pointer.numberPointer = new Number(val);
    }
    
    Object::Object(Object *left, Object *right) {
        _type = kObjectTypeCons;
        pointer.consValue.left = left;
        pointer.consValue.right = right;
    }
    
    Object::Object(ObjectList objects) {
        _type = kObjectTypeVector;
        pointer.vectorPointer = new ObjectList(objects);
    }
    
    std::string Object::stringValue() {
        return *pointer.stringValue;
    }
    
    ObjectList Object::vectorValue() {
        return *pointer.vectorPointer;
    }
    
    Number Object::numberValue() {
        return *pointer.numberPointer;
    }
    
    bool Object::booleanValue() {
        return pointer.booleanValue;
    }
    
    bool Object::coerceBoolean() {
        switch (_type) {                
            case kObjectTypeNumber:
                return numberValue()!=Number(0);
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
            case kObjectTypeBuiltinFunction:
                stringBuilder   << "<<<builtin "
                                << pointer.builtinFunctionValue.extensionFunctionPointer->functionName()
                                << ">>>";
                break;
                
            case kObjectTypeClosure:
                stringBuilder   << "<<<fn "
                                << pointer.functionValue.objectPointer->stringRepresentation()
                                << ">>>";
                break;
                
            case kObjectTypeString:
                stringBuilder << '"' << *pointer.stringValue << '"';
                break;
                
            case kObjectTypeNumber:
                stringBuilder << pointer.numberPointer->stringRepresentation();
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
                ObjectList elements;
                
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
    
    bool Object::isIterable() {
        if (_type == kObjectTypeVector) {
            return false;
        }
        
        if (isList()) {
            return true;
        }
                
        return false;
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
    
    bool Object::buildList(ObjectList& results) {
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
    
    Object* TinyClojure::listObject(ObjectList list) {
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
        
        for (char excludeChar = 1; excludeChar<32; ++excludeChar) {
            _excludeSet.append(&excludeChar, 1);
        }
        _excludeSet.append("\"()[]{}';` ");
        
        _numberSet = std::string("0123456789");
        
        _baseScope = NULL;
        
        loadExtensionFunctions();

        // this will initialise the root scope
        resetInterpreter();
    }
    
    TinyClojure::~TinyClojure() {
        for (std::vector<ExtensionFunction*>::iterator it = _extensionFunctions.begin(); it != _extensionFunctions.end(); ++it) {
            delete *it;
        }
        
        delete _baseScope;
        delete _ioProxy;
        delete _gc;
    }
    
    void TinyClojure::addExtensionFunction(ExtensionFunction *function) {
        internalAddExtensionFunction(function);
        resetInterpreter();
    }
    
    void TinyClojure::internalAddExtensionFunction(ExtensionFunction *function) {
        function->evaluator(this);
        function->garbageCollector(_gc);
        function->setIOProxy(_ioProxy);
        function->setup();
        
        _extensionFunctions.push_back(function);        
    }
    
    void TinyClojure::resetInterpreter() {
        if (_baseScope) {
            delete _baseScope;
        }
        
        _baseScope = new InterpreterScope();
        
        for (int functionIndex = 0; functionIndex < _extensionFunctions.size(); ++functionIndex) {
            ExtensionFunction *aFunction = _extensionFunctions[functionIndex];
            _baseScope->setSymbolInScope(aFunction->functionName(),
                                         _gc->registerObject(new Object(aFunction)));
        }
    }
    
    void TinyClojure::loadExtensionFunctions() {
        internalAddExtensionFunction(new core::Plus());
        internalAddExtensionFunction(new core::Minus());
        internalAddExtensionFunction(new core::Multiply());
        internalAddExtensionFunction(new core::Divide());
        internalAddExtensionFunction(new core::If());
        internalAddExtensionFunction(new core::Equality());
        internalAddExtensionFunction(new core::NotEqual());
        internalAddExtensionFunction(new core::Cons());
        internalAddExtensionFunction(new core::List());
        internalAddExtensionFunction(new core::LessThan);
        internalAddExtensionFunction(new core::LessThanOrEqual);
        internalAddExtensionFunction(new core::GreaterThan);
        internalAddExtensionFunction(new core::GreaterThanOrEqual);
        internalAddExtensionFunction(new core::Print());
        internalAddExtensionFunction(new core::Def);
        internalAddExtensionFunction(new core::Do);
        internalAddExtensionFunction(new core::Vector);
        internalAddExtensionFunction(new core::Fn);
        internalAddExtensionFunction(new core::Defn);
        internalAddExtensionFunction(new core::First);
        internalAddExtensionFunction(new core::Rest);
        internalAddExtensionFunction(new core::ReadString);
        internalAddExtensionFunction(new core::Eval);
        internalAddExtensionFunction(new core::ReadLine);
        internalAddExtensionFunction(new core::Cond);
        internalAddExtensionFunction(new core::Let);
    }
    
#pragma mark parser
    
    Object* TinyClojure::parse(std::string startText) {
        ParserState parseState(startText);
        return recursiveParse(parseState);
    }
    
    void TinyClojure::parseAll(std::string codeText, ObjectList& expressions) {
        ParserState parseState(codeText);
        
        while (parseState.charactersLeft()) {
            Object *code = recursiveParse(parseState);
            if (code) {
                expressions.push_back(code);
            }
        }
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
            return _gc->registerObject(new Object());
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
            ObjectList elements;
            
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
                    elements.insert(elements.begin(), _gc->registerObject(new Object(std::string("list"), true)));
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
            ObjectList elements;
            
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
            
            ObjectList elements;
            
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
                        
                        bool    isFloat = true,
                                pointFound = false;
                        for (int identifierIndex = 0; identifierIndex < identifier.size(); ++identifierIndex) {
                            if (identifier[identifierIndex]=='.') {
                                if (pointFound) {
                                    isFloat = false;
                                    break;
                                } else {
                                    pointFound = true;
                                }
                            } else if (_numberSet.find(identifier[identifierIndex]) == _numberSet.npos) {
                                isInteger = false;
                                break;
                            }
                        }
                        if (!pointFound) {
                            isFloat = false;
                        }
                        
                        if (isInteger) {
                            return _gc->registerObject(new Object(atoi(identifier.c_str())));
                        } else if (isFloat) {
                            return _gc->registerObject(new Object(atof(identifier.c_str())));
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
    
    Object* TinyClojure::scopedEval(InterpreterScope *interpreterState, Object *code) {
        InterpreterScope newScope(interpreterState);
        
        return unscopedEval(interpreterState, code);
    }
    
    Object* TinyClojure::unscopedEval(InterpreterScope *interpreterState, Object *code) {
        switch (code->type()) {
            case Object::kObjectTypeNil:
            case Object::kObjectTypeNumber:
            case Object::kObjectTypeString:
            case Object::kObjectTypeBoolean:
            case Object::kObjectTypeBuiltinFunction:
            case Object::kObjectTypeClosure:
                return code;
                break;
                
            case Object::kObjectTypeVector: {
                ObjectList elements;
                
                for (int elementIndex=0; elementIndex<code->vectorValue().size(); ++elementIndex) {
                    elements[elementIndex] = scopedEval(interpreterState, code->vectorValue()[elementIndex]);
                }
                
                return _gc->registerObject(new Object(elements));
                } break;
        
            case Object::kObjectTypeSymbol: {
                Object *symbolValue = interpreterState->lookupSymbol(code->stringValue());
                if (symbolValue) {
                    return symbolValue;
                } else {
                    std::stringstream stringBuilder;
                    stringBuilder << "I do not understand the symbol " << code->stringValue();
                    throw Error(stringBuilder.str());
                }
            } break;
        
            case Object::kObjectTypeCons:
                if (code->isList()) {
                    ObjectList arguments;
                    code->buildList(arguments);
                    
                    Object *identifierObject = scopedEval(interpreterState, arguments[0]);
                    
                    // remove the identifier so that arguments contains just the arguments to the function
                    arguments.erase(arguments.begin());
                    
                    if (identifierObject->type()==Object::kObjectTypeBuiltinFunction) {
                        ExtensionFunction *function = identifierObject->functionValueExtensionFunction();
                        
                        int minArgs = function->minimumNumberOfArguments(),
                            maxArgs = function->maximumNumberOfArguments();
                        
                        if (minArgs>=0) {
                            if (arguments.size() < minArgs) {
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
                            if (arguments.size() > maxArgs) {
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
                        
                        std::vector<Object::ObjectType> types;
                        for (int parameterIndex=0; parameterIndex<arguments.size(); ++parameterIndex) {
                            types.push_back(arguments[parameterIndex]->type());
                        }
                        
                        if (!function->validateArgumentTypes(types)) {
                            std::stringstream stringBuilder;
                            stringBuilder   << "Function "
                                            << function->functionName()
                                            << "'s type signature does not match that which is passed"
                                            << std::endl;

                            throw Error(stringBuilder.str());
                        }
                        
                        ObjectList preparedArguments;
                        if (function->preEvaluateArguments()) {
                            for (int argumentIndex=0; argumentIndex<arguments.size(); ++argumentIndex) {
                                preparedArguments.push_back(scopedEval(interpreterState, arguments[argumentIndex]));
                            }
                        } else {
                            preparedArguments = arguments;
                        }
                        
                        Object *result = function->execute(preparedArguments, interpreterState);
                        if (result==NULL) {
                            result = _gc->registerObject(new Object());
                        }
                        
                        return result;
                    } else if (identifierObject->type() == Object::kObjectTypeClosure) {
                        if (identifierObject->functionValueParameters().size()!=arguments.size()) {
                            std::stringstream stringBuilder;
                            stringBuilder   << "Function requires "
                                            << arguments.size()
                                            << " arguments"
                                            << std::endl;
                                            
                            throw Error(stringBuilder.str());
                        }
                                                                                
                        // build a new scope containing the passed arguments
                        InterpreterScope functionScope(interpreterState);
                        for (int parameterIndex=0; parameterIndex<identifierObject->functionValueParameters().size(); ++parameterIndex) {
                            functionScope.setSymbolInScope(identifierObject->functionValueParameters()[parameterIndex]->stringValue(), scopedEval(interpreterState, arguments[parameterIndex]));
                        }
                        
                        return scopedEval(&functionScope, identifierObject->functionValueCode());
                    } else {
                        throw Error("An executable S Expression must begin with a function object");
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
        Object *ret = unscopedEval(_baseScope, code);
        
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
    
    ParserState::ParserState(std::string& stringin) : parserString(stringin) {
        position = 0;
    }
    
    bool ParserState::charactersLeft() {
        return position < parserString.length();
    }
    
    int ParserState::skipNewLinesAndWhitespace() {
        return skipCharactersInString(" \n\r\t");
    }
    
    int ParserState::skipSeparators() {
        return skipCharactersInString(" \t,\n\r");
    }
    
    char ParserState::currentChar() {
        return parserString[position];
    }

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
#pragma mark -
#pragma mark IOProxy
    
    void IOProxy::writeOut(std::string stringout) {
        std::cout << stringout;
    }
    
    void IOProxy::writeErr(std::string stringout) {
        std::cerr << stringout;
    }
    
    std::string IOProxy::readLine() {
        std::string input;
        std::getline(std::cin, input, '\n');
        return input;
    }
}