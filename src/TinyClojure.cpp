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
#include <cstdarg>
#include <cstdio>
#include <cmath>
#include <fstream>

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

    Number::Number(Number* oldNum) {
        _mode = oldNum->_mode;
        _value = oldNum->_value;
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

    Number::NumberMode Number::getMode() const {
        return _mode;
    }

    void Number::roundUp() {
        if (_mode == kNumberModeFloating) {
            setInteger(std::ceil(_value.floating));
        }
    }

    void Number::roundDown() {
        if (_mode == kNumberModeFloating) {
            setInteger(std::floor(_value.floating));
        }
    }
    
#pragma mark - ExtensionFunction
    
    void ExtensionFunction::garbageCollector(GarbageCollector *gc_long, GarbageCollector *gc_short) {
        _gc_long = gc_long;
        _gc_short = gc_short;
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
                
                return _gc_short->registerObject(new Object(current));
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
                    falseBranch = _gc_short->registerObject(new Object());
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
                        return _gc_short->registerObject(new Object(false));
                    }
                }
                
                return _gc_short->registerObject(new Object(true));
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
                        return _gc_short->registerObject(new Object(false));
                    }
                }
                
                return _gc_short->registerObject(new Object(true));
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
                        return _gc_short->registerObject(new Object(false));
                    }
                }
                
                return _gc_short->registerObject(new Object(true));
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
                return _gc_short->registerObject(new Object(arguments));
            }
        };
        
        class List : public ExtensionFunction {
            std::string functionName() {
                return "list";
            }
            
            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {
                Object *null = _gc_short->registerObject(new Object());

                if (arguments.size()==0) {
                    return _gc_short->registerObject(new Object(null,null));
                } else if (arguments.size()==1) {
                    return _gc_short->registerObject(new Object(arguments[0],null));
                } else {
                    Object *rhs = null;
                    
                    for (long argumentIndex = arguments.size()-1; argumentIndex >= 0; --argumentIndex) {
                        Object *lhs = arguments[argumentIndex];
                        
                        rhs = _gc_short->registerObject(new Object(lhs, rhs));
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
                return _gc_short->registerObject(new Object(arguments[0], arguments[1]));
            }
        };
        
        class Print : public ExtensionFunction {
            std::string functionName() {
                return "print";
            }
            
            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {
                for (int argumentIndex=0; argumentIndex<arguments.size(); ++argumentIndex) {
                    std::string thisArgument = arguments[argumentIndex]->stringValue();
                    
                    _ioProxy->writeOut(thisArgument);

                    // Only print out a space as long as this is not the last argument to print
                    if (argumentIndex != (arguments.size()-1)) {
                        _ioProxy->writeOut(" ");
                    }
                }
                
                return _gc_short->registerObject(new Object());
            }
        };


        class Println : public ExtensionFunction {
            std::string functionName() {
                return "println";
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {
                for (int argumentIndex=0; argumentIndex<arguments.size(); ++argumentIndex) {
                    std::string thisArgument = arguments[argumentIndex]->stringValue();

                    _ioProxy->writeOut(thisArgument);

                    // Only print out a space as long as this is not the last argument to print
                    if (argumentIndex != (arguments.size()-1)) {
                        _ioProxy->writeOut(" ");
                    }
                }

                _ioProxy->writeOut("\n");

                return _gc_short->registerObject(new Object());
            }

        };

        class PrintStr : public ExtensionFunction {
            std::string functionName() {
                return "print-str";
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {

                std::string stringToPrint;

                for (int argumentIndex=0; argumentIndex<arguments.size(); ++argumentIndex) {
                    std::string thisArgument = arguments[argumentIndex]->stringValue();

                    stringToPrint.append(thisArgument);

                    // Only add a space as long as this is not the last argument to append
                    if (argumentIndex != (arguments.size()-1)) {
                        stringToPrint.append(" ");
                    }
                }

                return _gc_short->registerObject(new Object(stringToPrint));
            }

        };

        class PrintlnStr : public ExtensionFunction {
            std::string functionName() {
                return "println-str";
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {

                std::string stringToPrint;

                for (int argumentIndex=0; argumentIndex<arguments.size(); ++argumentIndex) {
                    std::string thisArgument = arguments[argumentIndex]->stringValue();

                    stringToPrint.append(thisArgument);

                    // Only add a space as long as this is not the last argument to append
                    if (argumentIndex != (arguments.size()-1)) {
                        stringToPrint.append(" ");
                    }
                }

                stringToPrint.append("\n");

                return _gc_short->registerObject(new Object(stringToPrint));
            }

        };

        class Str : public ExtensionFunction {
            std::string functionName() {
                return "str";
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterScope) {

                std::string result;

                // no arguments or only nil argument, return empty string
                if (arguments.size() == 0 || (arguments.size() == 1 && arguments[0]->type() == Object::kObjectTypeNil)) {
                    result = "";
                } else if (arguments.size() == 1) {
                    // one argument return the string representation of the input
                    result.append(arguments[0]->stringValue());
                } else {
                    // more than one argument concatenate the strings and return them
                    for (int argumentIndex = 0; argumentIndex<arguments.size(); ++argumentIndex) {
                        result.append(arguments[argumentIndex]->stringValue());
                    }
                }

                return _gc_short->registerObject(new Object(result));
            }

        };

        class Count : public ExtensionFunction {
            std::string functionName() {
                return "count";
            }

            int requiredNumberOfArguments() {
                return 1;
            }

            bool validateArgumentTypes(std::vector<Object::ObjectType>& typeArray) {

                if (typeArray[0] == Object::kObjectTypeNil || typeArray[0] == Object::kObjectTypeString || typeArray[0] == Object::kObjectTypeCons) {
                    return true;
                } else {
                    return false;
                }
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterScope) {

                int result;

                switch (arguments[0]->type()) {
                    case Object::kObjectTypeNil:
                        result = 0;
                        break;
                    case Object::kObjectTypeString:
                        result = arguments[0]->stringValue().length();
                        break;
                    case Object::kObjectTypeVector:
                        result = arguments[0]->vectorValue().size();
                        break;
                    default:
                        break;
                }

                return _gc_short->registerObject(new Object(result));

            }
        };

        class Compare : public ExtensionFunction {
            std::string functionName() {
                return "compare";
            }

            int requiredNumberOfArguments() {
                return 2;
            }

            bool validateArgumentTypes(std::vector<Object::ObjectType>& typeArray) {

                if (typeArray[0] == Object::kObjectTypeNil || typeArray[0] == Object::kObjectTypeString || typeArray[0] == Object::kObjectTypeCons) {
                    return true;
                } else {
                    return false;
                }
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterScope) {

                if ((arguments[0]->type() == Object::kObjectTypeString && arguments[1]->type() == Object::kObjectTypeVector) || (arguments[0]->type() == Object::kObjectTypeVector && arguments[1]->type() == Object::kObjectTypeString)) {
                    std::stringstream stringbuilder;

                    stringbuilder << "You may not compare a string and a vector";

                    throw Error(stringbuilder.str());
                }

                int result;


                if (arguments[0]->type() == Object::kObjectTypeNil) {
                    // If the first is nil

                    if (arguments[1]->type() == Object::kObjectTypeNil) {
                        // and the second we know they are the same
                        result = 0;
                    } else {
                        // If not nil then it is automatically lower than the rhs
                        result = -1;
                    }

                } else if (arguments[1]->type() == Object::kObjectTypeNil) {

                    // Lhs is automatically greater than rhs as lhs is not nil and rhs is nil
                    result = 1;

                } else {

                    if (arguments[0]->type() == Object::kObjectTypeString || arguments[0]->type() == Object::kObjectTypeString) {
                        // Handle comparing strings
                        result = arguments[0]->stringValue().compare(arguments[1]->stringValue());

                    } else {
                        result = arguments[0]->vectorValue().size() - arguments[1]->vectorValue().size();
                    }
                }

                return _gc_short->registerObject(new Object(result));
            }

        };

        class Subs : public ExtensionFunction {
            std::string functionName() {
                return "subs";
            }

            int minimumNumberOfArguments() {
                return 2;
            }

            int maximumNumberOfArguments() {
                return 3;
            }

            bool validateArgumentTypes(std::vector<Object::ObjectType>& typeArray) {

                if (typeArray[0] == Object::kObjectTypeString && typeArray[1] == Object::kObjectTypeNumber) {

                    if (typeArray.size() == 3) {
                        if (typeArray[2] == Object::kObjectTypeNumber) {
                            return true;
                        } else {
                            return false;
                        }
                    } else {
                        return true;
                    }
                } else {
                    return false;
                }
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterScope) {

                std::string result;

                if (arguments.size() == 3) {
                    result = arguments[0]->stringValue().substr(arguments[1]->numberValue().integerValue(), (arguments[2]->numberValue().integerValue()-arguments[1]->numberValue().integerValue()));
                } else {
                    result = arguments[0]->stringValue().substr(arguments[1]->numberValue().integerValue(), std::string::npos);
                }

                return _gc_short->registerObject(new Object(result));
            }

        };

        class Quot : public ExtensionFunction {
            std::string functionName() {
                return "quot";
            }

            int requiredNumberOfArguments() {
                return 2;
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {
                // Divide the two numbers passed as arguments
                Number result = arguments[0]->numberValue() / arguments[1]->numberValue();

                if (result.floatingValue() >= 0) {
                    // Round down to the nearest integer
                    result.roundDown();
                } else {
                    // Round up to the nearest integer
                    result.roundUp();
                }

                return _gc_short->registerObject(new Object(result.integerValue()));
            }
        };

        class Rem : public ExtensionFunction {
            std::string functionName() {
                return "rem";
            }

            int requiredNumberOfArguments() {
                return 2;
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {
                // Divide the two numbers passed as arguments
                Number result = arguments[0]->numberValue() / arguments[1]->numberValue();

                if (result.floatingValue() >= 0) {
                    // Round down to the nearest integer
                    result.roundDown();
                } else {
                    // Round up to the nearest integer
                    result.roundUp();
                }

                Number remainder = arguments[0]->numberValue() - (result * arguments[1]->numberValue());

                return _gc_short->registerObject(new Object(remainder));
            }
        };

        class Inc : public ExtensionFunction {
            std::string functionName() {
                return "inc";
            }

            int requiredNumberOfArguments() {
                return 1;
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {

                Number result = arguments[0]->numberValue() + Number(1);

                return _gc_short->registerObject(new Object(result));

            }
        };

        class Dec : public ExtensionFunction {
            std::string functionName() {
                return "dec";
            }

            int requiredNumberOfArguments() {
                return 1;
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {

                Number result = arguments[0]->numberValue() - Number(1);

                return _gc_short->registerObject(new Object(result));

            }
        };

        class Max : public ExtensionFunction {
            std::string functionName() {
                return "max";
            }

            int minimumNumberOfArguments() {
                return 1;
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {

                Number maxVal = arguments[0]->numberValue();

                for (int i=0; i<arguments.size(); i++) {

                    if (arguments[i]->numberValue() > maxVal) {
                        maxVal = arguments[i]->numberValue();
                    }
                }

                return _gc_short->registerObject(new Object(maxVal));
            }
        };

        class Min : public ExtensionFunction {
            std::string functionName() {
                return "min";
            }

            int minimumNumberOfArguments() {
                return 1;
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {

                Number minVal = arguments[0]->numberValue();

                for (int i=0; i<arguments.size(); i++) {

                    if (arguments[i]->numberValue() < minVal) {
                        minVal = arguments[i]->numberValue();
                    }
                }

                return _gc_short->registerObject(new Object(minVal));

            }
        };

        class LoadFile : public ExtensionFunction {
            std::string functionName() {
                return "load-file";
            }

            int requiredNumberOfArguments() {
                return 1;
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {

                std::ifstream myFile(arguments[0]->stringValue());

                // Error handling if unable to open the file
                if (myFile.fail()) {
                    std::cout << "Failed to open " << arguments[0]->stringValue() << std::endl;
                }

                std::string myLine;


                while (std::getline(myFile, myLine)) {

                    try {
                        Object *code = _evaluator->parse(myLine);

                        if (code) {
                            if (code->type() != tinyclojure::Object::kObjectTypeNil) {
                                Object *resultObject = _evaluator->scopedEval(interpreterState, code);
                                std::cout << resultObject->stringRepresentation() << std::endl;
                            }
                        }

                    } catch (tinyclojure::Error error) {
                        std::cout << error.position << ": " << error.message << std::endl << std::endl;
                    }

                }
                return _gc_short->registerObject(new Object());
            }
        };

        // Hanldes only one set per string
        class LoadString : public ExtensionFunction {
            std::string functionName() {
                return "load-string";
            }

            int requiredNumberOfArguments() {
                return 1;
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {

                try {
                    Object *code = _evaluator->parse(arguments[0]->stringValue());

                    if (code) {
                        if (code->type() != tinyclojure::Object::kObjectTypeNil) {
                            return _gc_short->registerObject(_evaluator->scopedEval(interpreterState, code));
                        }
                    }

                } catch (tinyclojure::Error error) {
                    std::cout << error.position << ": " << error.message << std::endl << std::endl;
                }

                return _gc_short->registerObject(new Object());
            }
        };

        class Spit : public ExtensionFunction {
            std::string functionName() {
                return "spit";
            }

            int minimumNumberOfArguments() {
                return 2;
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {

                std::ofstream myFile(arguments[0]->stringValue());
                myFile << arguments[1]->stringValue();

                return _gc_short->registerObject(new Object());
            }
        };

        class Slurp : public ExtensionFunction {
            std::string functionName() {
                return "slurp";
            }

            int minimumNumberOfArguments() {
                return 1;
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {

                std::ifstream myFile(arguments[0]->stringValue());
                std::string myLine;
                std::string result;

                while (std::getline(myFile, myLine)) {
                    result += myLine;
                }

                return _gc_short->registerObject(new Object(result));
            }
        };

        class Mod : public ExtensionFunction {
            std::string functionName() {
                return "mod";
            }

            int requiredNumberOfArguments() {
                return 2;
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {

                // Divide the two numbers passed as arguments
                Number result = arguments[0]->numberValue() / arguments[1]->numberValue();

                if (result.floatingValue() >= 0) {
                    // Round down to the nearest integer
                    result.roundDown();
                } else {
                    // Round up to the nearest integer
                    result.roundUp();
                }

                Number remainder = arguments[0]->numberValue() - (result * arguments[1]->numberValue());

                return _gc_short->registerObject(new Object(remainder));

            }
        };

        // Ns-unmap function from Clojure core library
        // Currently, does not take namespace as an argument (only the symbol to be undefined), as only the core library is implemented
        // In the future if new namespaces are added, this function will need to be redone
        class Nsunmap : public ExtensionFunction {
            std::string functionName() {
                return "ns-unmap";
            }

            int requiredNumberOfArguments() {
                return 1;
            }

            bool preEvaluateArguments() {
                return false;
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {

                // Erases the symbol from the symbol table
                // Returns the corresponding object for that symbol (for deletion in the garbage collector)
                Object *ret = interpreterState->removeSymbol(arguments[0]->stringValue());

                // Delete the returned object from the garbage collector
                _gc_long->deleteObject(ret);

                return _gc_short->registerObject(new Object());
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
                Object  *symbol = _gc_long->registerObject(new Object(arguments[0], _gc_long)),
                        *value = _gc_long->registerObject(new Object(_evaluator->scopedEval(interpreterState, arguments[1]), _gc_long));
                
                if (symbol->type()!=Object::kObjectTypeSymbol) {
                    throw Error("first argument to def must be a symbol");
                }
                
                interpreterState->setSymbolInScope(symbol->stringValue(), value);
                
                return _gc_short->registerObject(new Object());
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
                
                Object  *retValue = _gc_short->registerObject(new Object());
                
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
                        
                        return _gc_short->registerObject(new Object(left, right));
                    } break;
                        
                    case Object::kObjectTypeVector: {
                        std::vector<Object *> newVector;
                        
                        for (int vectorIndex = 0; vectorIndex < object->vectorValue().size(); ++vectorIndex) {
                            newVector.push_back(captureState(object->vectorValue()[vectorIndex], interpreterState));
                        }
                        
                        return _gc_short->registerObject(new Object(newVector));
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
                Object  *symbol = _gc_long->registerObject(new Object(arguments[0], _gc_long)),
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
                capturedArguments.push_back(_gc_short->registerObject(new Object("do", true)));
                for (int argumentIndex = 0; argumentIndex < arguments.size(); ++argumentIndex) {
                    capturedArguments.push_back(captureState(arguments[argumentIndex], interpreterState));
                }
                
                Object *lambda = _gc_short->registerObject(new Object(_evaluator->listObject(capturedArguments), argumentSymbols));
                Object* lambda_long = _gc_long->registerObject(new Object(lambda, _gc_long));
                
                interpreterState->setSymbolInScope(symbol->stringValue(), lambda_long);
                
                return _gc_short->registerObject(new Object());
            }
        };

        class Defmacro : public Closure {
            std::string functionName() {
                return "defmacro";
            }

            int minimumNumberOfArguments() {
                return 3;
            }

            Object *execute(ObjectList arguments, InterpreterScope *interpreterState) {

                // Differentiate between symbols and arg list
                Object *symbol = _gc_long->registerObject(new Object(arguments[0], _gc_long)), *argList = arguments[1];

                if (symbol->type()!=Object::kObjectTypeSymbol) {
                    throw Error("first argument to defmacro must be a symbol");
                }

                // construct an argument list
                ObjectList argumentSymbols;
                constructArgumentList(argList, argumentSymbols);

                // remove the initial argument and symbol, just leaving the function body
                arguments.erase(arguments.begin());
                arguments.erase(arguments.begin());

                // capture the arguments
                ObjectList capturedArguments;
                capturedArguments.push_back(_gc_short->registerObject(new Object("do", true)));
                for (int argumentIndex = 0; argumentIndex < arguments.size(); ++argumentIndex) {
                    capturedArguments.push_back(captureState(arguments[argumentIndex], interpreterState));
                }

                Object *lambda = _gc_short->registerObject(new Object(_evaluator->listObject(capturedArguments), argumentSymbols, true));
                Object* lambda_long = _gc_long->registerObject(new Object(lambda, _gc_long));

                interpreterState->setSymbolInScope(symbol->stringValue(), lambda_long);

                return _gc_short->registerObject(new Object());
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
                capturedArguments.push_back(_gc_short->registerObject(new Object("do", true)));
                for (int argumentIndex = 0; argumentIndex < arguments.size(); ++argumentIndex) {
                    capturedArguments.push_back(captureState(arguments[argumentIndex], interpreterState));
                }
                
                return _gc_long->registerObject(new Object(_gc_short->registerObject(new Object(_evaluator->listObject(capturedArguments), argumentSymbols)), _gc_long));
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
                return _gc_short->registerObject(new Object(_ioProxy->readLine()));
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
                
                return _gc_short->registerObject(new Object());
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

                    Object* newBindVal = _gc_long->registerObject(new Object(evaluatedBindingValue, _gc_long));

                    letScope.setSymbolInScope(bindingSymbol->stringValue(), newBindVal);
                }
                
                // now evaluate the arguments in turn
                Object  *retValue = _gc_short->registerObject(new Object());
                
                for (int argumentIndex = 1; argumentIndex < arguments.size(); ++argumentIndex) {
                    retValue = _evaluator->unscopedEval(&letScope, arguments[argumentIndex]);
                }
                
                return retValue;
            }
        };
        
        class Nth : public ExtensionFunction {
            std::string functionName() {
                return "nth";
            }
            
            int minimumNumberOfArguments() {
                return 2;
            }
            
            int maximumNumberOfArguments() {
                return 3;
            }
            
            Object* execute(ObjectList arguments, InterpreterScope *interpreterState) {
                Object  *collection = arguments[0],
                        *indexValue = arguments[1],
                        *defaultValue = NULL;
                
                if (arguments.size()==3) {
                    defaultValue = arguments[2];
                }
                
                if (indexValue->type()!=Object::kObjectTypeNumber) {
                    throw Error("second argument to nth must be an index");
                }
                
                const int index = indexValue->numberValue().integerValue();
                
                ObjectList convertedList;
                if (!collection->buildList(convertedList)) {
                    throw Error("first argument to nth must be a collection");
                }
                
                if (index < convertedList.size()) {
                    if (index >= 0) {
                        return convertedList[index];
                    } else {
                        throw Error("index to nth is < 0");
                    }
                } else {
                    // out of bounds.  this is an exception if no default value is supplied
                    if (defaultValue) {
                        return defaultValue;
                    } else {
                        throw Error("index to nth is out of bounds");
                    }
                }
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

    Object* InterpreterScope::removeSymbolInScope(std::string symbolName) {
        Object* ret = lookupSymbolInScope(symbolName);
        _symbolTable.erase(symbolName);
        return ret;
    }

    Object* InterpreterScope::removeSymbol(std::string symbolName) {
        Object* ret = removeSymbolInScope(symbolName);

        if (ret) {
            return ret;
        } else {
            if (_parentScope) {
                return _parentScope->removeSymbol(symbolName);
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
            _contents.stringValue = new std::string(stringVal);            
        } else {
            _type = kObjectTypeString;
            _contents.stringValue = new std::string(stringVal);
        }
    }
    
    Object::Object(Object *code, ObjectList arguments) {
        _type = kObjectTypeClosure;
        _contents.functionValue.objectPointer = code;
        _contents.functionValue.argumentSymbols = new ObjectList(arguments);
        _contents.functionValue.macro = false;
    }

    Object::Object(Object *code, ObjectList arguments, bool macro) {
        _type = kObjectTypeClosure;
        _contents.functionValue.objectPointer = code;
        _contents.functionValue.argumentSymbols = new ObjectList(arguments);
        _contents.functionValue.macro = macro;
    }

    Object::Object(ExtensionFunction *function) {
        _type = kObjectTypeBuiltinFunction;
        _contents.builtinFunctionValue.extensionFunctionPointer = function;
    }

    // Creates a deep copy of an object
    // Does not have the ability to clone built in functions
    Object::Object(Object* oldObj, GarbageCollector* gc) {

        _type = oldObj->_type;

        switch (_type) {

            case kObjectTypeSymbol:
                _contents.stringValue = new std::string(oldObj->stringValue());
                break;

            case kObjectTypeString:
                _contents.stringValue = new std::string(oldObj->stringValue());
                break;

            case kObjectTypeVector:
                _contents.vectorPointer = new ObjectList();

                for(unsigned i = 0; i < oldObj->_contents.vectorPointer->size(); ++i)
                    _contents.vectorPointer->push_back(gc->registerObject(new Object(oldObj->_contents.vectorPointer->at(i), gc)));
                break;

            case kObjectTypeClosure:
                _contents.functionValue.objectPointer = gc->registerObject(new Object(oldObj->_contents.functionValue.objectPointer, gc));

                _contents.functionValue.argumentSymbols = new ObjectList();

                for(unsigned i = 0; i < oldObj->_contents.functionValue.argumentSymbols->size(); ++i)
                    _contents.functionValue.argumentSymbols->push_back(gc->registerObject(new Object(oldObj->_contents.functionValue.argumentSymbols->at(i), gc)));

                _contents.functionValue.macro = oldObj->_contents.functionValue.macro;
                break;

            case kObjectTypeNumber:
                _contents.numberPointer = new Number(_contents.numberPointer);
                break;

            case kObjectTypeCons:
                _contents.consValue.left = gc->registerObject(new Object(oldObj->consValueLeft(), gc));
                _contents.consValue.right = gc->registerObject(new Object(oldObj->consValueRight(), gc));
                break;

            // Does not deep copy built in functions
            case kObjectTypeBuiltinFunction:
                _contents.builtinFunctionValue = oldObj->_contents.builtinFunctionValue;
                break;

            case kObjectTypeBoolean:
                _contents.booleanValue = _contents.booleanValue;
                break;

            case kObjectTypeNil:
                break;
        }
    }

    Object::~Object() {
        switch (_type) {
            case kObjectTypeSymbol:
            case kObjectTypeString:
                delete _contents.stringValue;
                break;
                                
            case kObjectTypeVector:
                delete _contents.vectorPointer;
                break;
            
            case kObjectTypeClosure:
                // leave the Objects to the gc
                delete _contents.functionValue.argumentSymbols;
                break;

            case kObjectTypeNumber:
                delete _contents.numberPointer;
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
                return _contents.booleanValue == rhs._contents.booleanValue;
                break;
                
            case kObjectTypeNumber:
                return *_contents.numberPointer == *rhs._contents.numberPointer;
                break;
                
            case kObjectTypeNil:
                return true;
                break;
                
            case kObjectTypeVector:
                if (_contents.vectorPointer->size() == rhs._contents.vectorPointer->size()) {
                    for (int elementIndex=0; elementIndex < _contents.vectorPointer->size(); ++elementIndex) {
                        if (*(_contents.vectorPointer->at(elementIndex)) != *(rhs._contents.vectorPointer->at(elementIndex))) {
                            return false;
                        }
                        
                    }
                    return true;
                } else {
                    return false;
                }
                break;
                
            case kObjectTypeBuiltinFunction:
                return _contents.builtinFunctionValue.extensionFunctionPointer->functionName() == rhs._contents.builtinFunctionValue.extensionFunctionPointer->functionName();
                break;
                
            case kObjectTypeClosure:
                return *_contents.functionValue.objectPointer == *rhs._contents.functionValue.objectPointer;
                break;
 
            case kObjectTypeSymbol:
            case kObjectTypeString:
                return *_contents.stringValue == *rhs._contents.stringValue;
                break;
            
            case kObjectTypeCons:
                // recursively evaluate ==, NB lhs equality should return more quickly than rhs equality
                if (*_contents.consValue.left != *rhs._contents.consValue.left) {
                    return false;
                }
                return *_contents.consValue.right == *rhs._contents.consValue.right;
                break;
        }
    }
    
    bool Object::operator!=(const Object& rhs) {
        return !operator==(rhs);
    }
    
    Object* Object::functionValueCode() {
        return _contents.functionValue.objectPointer;
    }

    ExtensionFunction* Object::functionValueExtensionFunction() {
        return _contents.builtinFunctionValue.extensionFunctionPointer;
    }
    
    ObjectList Object::functionValueParameters() {
        return *_contents.functionValue.argumentSymbols;
    }

    bool Object::isMacro() {
        return _contents.functionValue.macro;
    }
    
    Object* Object::consValueLeft() {
        return _contents.consValue.left;
    }
    
    Object* Object::consValueRight() {
        return _contents.consValue.right;
    }
    
    Object::Object(Number numberValue) {
        _type = kObjectTypeNumber;
        _contents.numberPointer = new Number(numberValue);
    }
    
    Object::Object(bool boolValue) {
        _type = kObjectTypeBoolean;
        _contents.booleanValue = boolValue;
    }
    
    Object::Object(int val) {
        _type = kObjectTypeNumber;
        _contents.numberPointer = new Number(val);
    }

    Object::Object(double val) {
        _type = kObjectTypeNumber;
        _contents.numberPointer = new Number(val);
    }
    
    Object::Object(Object *left, Object *right) {
        _type = kObjectTypeCons;
        _contents.consValue.left = left;
        _contents.consValue.right = right;
    }
    
    Object::Object(ObjectList objects) {
        _type = kObjectTypeVector;
        _contents.vectorPointer = new ObjectList(objects);
    }
    
    std::string Object::stringValue(bool expandList) {
        std::stringstream stringBuilder;

        switch (_type) {
            case kObjectTypeBuiltinFunction:
                stringBuilder << "<<<builtin "
                << _contents.builtinFunctionValue.extensionFunctionPointer->functionName()
                << ">>>";
                break;

            case kObjectTypeClosure:
                stringBuilder << "<<<fn "
                << _contents.functionValue.objectPointer->stringRepresentation()
                << ">>>";
                break;

            case kObjectTypeString:
                stringBuilder << *_contents.stringValue;
                break;

            case kObjectTypeNumber:
                stringBuilder << _contents.numberPointer->stringRepresentation();
                break;

            case kObjectTypeVector:
                stringBuilder << "[";
                for (int elementIndex = 0; elementIndex < vectorValue().size(); ++elementIndex) {
                    if (elementIndex) {
                        stringBuilder << " ";
                    }
                    stringBuilder << _contents.vectorPointer->at(elementIndex)->stringRepresentation();
                }
                stringBuilder << "]";
                break;

            case kObjectTypeBoolean:
                if (_contents.booleanValue) {
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

                        if (listIndex < elements.size() - 1) {
                            stringBuilder << " ";
                        }
                    }
                    stringBuilder << ")";
                } else {
                    stringBuilder << "(cons "
                    << _contents.consValue.left->stringRepresentation()
                    << " "
                    << _contents.consValue.right->stringRepresentation()
                    << ")";
                }
            }
                break;

            case kObjectTypeNil:
                stringBuilder << "nil";
                break;

            case kObjectTypeSymbol:
                stringBuilder << *_contents.stringValue;
                break;
        }

        return stringBuilder.str();
    }
    
    ObjectList Object::vectorValue() {
        return *_contents.vectorPointer;
    }
    
    Number Object::numberValue() {
        return *_contents.numberPointer;
    }
    
    bool Object::booleanValue() {
        return _contents.booleanValue;
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
                                << _contents.builtinFunctionValue.extensionFunctionPointer->functionName()
                                << ">>>";
                break;
                
            case kObjectTypeClosure:
                stringBuilder   << "<<<fn "
                                << _contents.functionValue.objectPointer->stringRepresentation()
                                << ">>>";
                break;
                
            case kObjectTypeString:
                stringBuilder << '"' << *_contents.stringValue << '"';
                break;
                
            case kObjectTypeNumber:
                stringBuilder << _contents.numberPointer->stringRepresentation();
                break;
                
            case kObjectTypeVector:
                stringBuilder << "[";
                for (int elementIndex=0; elementIndex<vectorValue().size(); ++elementIndex) {
                    if (elementIndex) {
                        stringBuilder << " ";
                    }
                    stringBuilder << _contents.vectorPointer->at(elementIndex)->stringRepresentation();
                }
                stringBuilder << "]";
                break;
                
            case kObjectTypeBoolean:
                if (_contents.booleanValue) {
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
                                    << _contents.consValue.left->stringRepresentation()
                                    << " "
                                    << _contents.consValue.right->stringRepresentation()
                                    << ")";
                }
                } break;
                
            case kObjectTypeNil:
                stringBuilder << "nil";
                break;
                
            case kObjectTypeSymbol:
                stringBuilder << *_contents.stringValue;
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
            if (currentObject->_contents.consValue.right->_type == kObjectTypeCons) {
                // this is a cons with a cons as its right value, continue the list
                currentObject = currentObject->_contents.consValue.right;
            } else if (currentObject->_contents.consValue.right->_type == kObjectTypeNil) {
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
        if (_type == kObjectTypeVector) {
            results = *_contents.vectorPointer;
            
            return true;
        } else {
            Object *currentObject = this;

            results.clear();
            
            while (currentObject->_type == kObjectTypeCons) {
                if (currentObject->_contents.consValue.right->_type == kObjectTypeCons) {
                    // this is a cons with a cons as its right value, continue the list
                    results.push_back(currentObject->_contents.consValue.left);
                    currentObject = currentObject->_contents.consValue.right;
                } else if (currentObject->_contents.consValue.right->_type == kObjectTypeNil) {
                    // a nil terminator for the list
                    results.push_back(currentObject->_contents.consValue.left);
                    return true;
                } else {
                    // this isn't a list
                    return false;
                }
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
                Object *nilObject = _gc_short->registerObject(new Object());
                return _gc_short->registerObject(new Object(list[0], nilObject));
            } else {
                Object *left = list[0];
                list.erase(list.begin());
                return _gc_short->registerObject(new Object(left, listObject(list)));
            }
        } else {
            // clojure's empty lists seem to be (cons nil nil)
            Object *nilObject = _gc_short->registerObject(new Object());
            return _gc_short->registerObject(new Object(nilObject, nilObject));
        }
    }

    
    TinyClojure::TinyClojure() {
        _ioProxy = new IOProxy();
        _gc_long = new GarbageCollector();
        _gc_short = new GarbageCollector();
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
        delete _gc_long;
        delete _gc_short;
    }
    
    void TinyClojure::addExtensionFunction(ExtensionFunction *function) {
        internalAddExtensionFunction(function);
        resetInterpreter();
    }
    
    void TinyClojure::internalAddExtensionFunction(ExtensionFunction *function) {
        function->evaluator(this);
        function->garbageCollector(_gc_long, _gc_short);
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
                                         _gc_long->registerObject(new Object(aFunction)));
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
        internalAddExtensionFunction(new core::Println());
        internalAddExtensionFunction(new core::PrintStr());
        internalAddExtensionFunction(new core::PrintlnStr());
        internalAddExtensionFunction(new core::Str());
        internalAddExtensionFunction(new core::Count());
        internalAddExtensionFunction(new core::Compare());
        internalAddExtensionFunction(new core::Subs());
        internalAddExtensionFunction(new core::Quot());
        internalAddExtensionFunction(new core::Rem());
        internalAddExtensionFunction(new core::Mod());
        internalAddExtensionFunction(new core::Inc());
        internalAddExtensionFunction(new core::Dec());
        internalAddExtensionFunction(new core::Max());
        internalAddExtensionFunction(new core::Min());
        internalAddExtensionFunction(new core::LoadFile());
        internalAddExtensionFunction(new core::LoadString());
        internalAddExtensionFunction(new core::Spit());
        internalAddExtensionFunction(new core::Slurp());
        internalAddExtensionFunction(new core::Nsunmap());
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
        internalAddExtensionFunction(new core::Nth);
        internalAddExtensionFunction(new core::Defmacro);
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
            return _gc_short->registerObject(new Object());
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
                ++parseState.position;
                
                if (escapeNextChar) {
                    char adjustedChar = currentChar;
                    
                    switch (currentChar) {
                        case 'n':
                            adjustedChar = 10;
                            break;
                            
                        case 'r':
                            adjustedChar = 13;
                            break;
                            
                        case 't':
                            adjustedChar = 9;
                            break;
                    }
                    
                    stringbuf.append(&adjustedChar, 1);

                    escapeNextChar = false;
                } else {
                    if (currentChar == '\\') {
                        escapeNextChar = true;
                    } else {
                        if (currentChar=='"' && stringbuf.length()) {
                            // TODO do something in the case of regexes
                            if (stringType == stringTypeRegex) {
                                std::cerr << "Not dealing with regexes" << std::endl;
                            }
                            
                            // end of the string
                            Object *element = new Object(stringbuf);
                            _gc_short->registerObject(element);
                            return element;
                        }
                        stringbuf.append(&currentChar, 1);
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
                    elements.insert(elements.begin(), _gc_short->registerObject(new Object(std::string("list"), true)));
                    return listObject(elements);
                    break;
                    
                case sexpTypeLambdaShorthand:
                    _ioProxy->writeErr("lambda shorthand unimplemented");
                    return NULL;
                    break;
                    
                case sexpTypeHashSet:
                    elements.insert(elements.begin(), _gc_short->registerObject(new Object(std::string("hash-set", true))));
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
                    elements.insert(elements.begin(), _gc_short->registerObject(new Object("vector", true)));
                    
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
                    els.push_back(_gc_short->registerObject(new Object("quote", true)));
                    els.push_back(_gc_short->registerObject(new Object(symbol, true)));
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
                            return _gc_short->registerObject(new Object(true));
                        } else if (identifier == "false") {
                            return _gc_short->registerObject(new Object(false));
                        } else if (identifier == "nil") {
                            return _gc_short->registerObject(new Object());
                        }
                        
                        int numberBaseIndex = 0;
                        if (identifier[numberBaseIndex] == '-') {
                            numberBaseIndex = 1;
                        }
                        
                        bool isInteger = true;
                        for (int identifierIndex = numberBaseIndex; identifierIndex < identifier.size(); ++identifierIndex) {
                            if (_numberSet.find(identifier[identifierIndex]) == _numberSet.npos) {
                                isInteger = false;
                                break;
                            }
                        }
                        if (identifier.length() == numberBaseIndex) {
                            // this can happen with a pure - sign
                            isInteger = false;
                        }
                        
                        bool    isFloat = true,
                                pointFound = false;
                        for (int identifierIndex = numberBaseIndex; identifierIndex < identifier.size(); ++identifierIndex) {
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
                            return _gc_short->registerObject(new Object(atoi(identifier.c_str())));
                        } else if (isFloat) {
                            return _gc_short->registerObject(new Object(atof(identifier.c_str())));
                        } else {
                            return _gc_short->registerObject(new Object(identifier, true));
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
                
                return _gc_short->registerObject(new Object(elements));
                } break;
        
            case Object::kObjectTypeSymbol: {

                Object *symbolValue = interpreterState->lookupSymbol(code->stringValue());

                if (symbolValue) {

                    // Checks if the symbol still needs to be evaluated (i.e. Macros need to be evaluated at a different stage)
                    if (symbolValue->type() == Object::kObjectTypeCons) {
                        if (symbolValue->consValueLeft()->stringValue() == "macroEval") {

                            if (symbolValue->consValueRight()->type() == Object::kObjectTypeCons) {
                                Object *temp = scopedEval(interpreterState, symbolValue->consValueRight());
                                return scopedEval(interpreterState, temp);
                            } else {
                                return symbolValue->consValueRight();
                            }
                        }
                    }

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
                            result = _gc_short->registerObject(new Object());
                        }
                        
                        return result;
                    } else if (identifierObject->type() == Object::kObjectTypeClosure) {

                        if (identifierObject->functionValueParameters().size() != arguments.size()) {
                            std::stringstream stringBuilder;
                            stringBuilder << "Function requires "
                            << identifierObject->functionValueParameters().size()
                            << " argument(s)"
                            << std::endl;

                            throw Error(stringBuilder.str());
                        }

                        // build a new scope containing the passed arguments
                        InterpreterScope functionScope(interpreterState);

                        if (identifierObject->isMacro()) {

                            // is a macro
                            for (int parameterIndex = 0; parameterIndex < identifierObject->functionValueParameters().size(); ++parameterIndex) {
                                std::string macroEval = "macroEval";

                                Object* testObj = _gc_short->registerObject(new Object(_gc_short->registerObject(new Object(macroEval)), parse(arguments[parameterIndex]->stringValue())));
                                Object* newTestObj = _gc_long->registerObject(new Object(testObj, _gc_long));
                                functionScope.setSymbolInScope(identifierObject->functionValueParameters()[parameterIndex]->stringValue(), newTestObj);
                            }

                        } else {

                            // not a macro, normal function
                            for (int parameterIndex = 0; parameterIndex < identifierObject->functionValueParameters().size(); ++parameterIndex) {
                                functionScope.setSymbolInScope(identifierObject->functionValueParameters()[parameterIndex]->stringValue(), _gc_long->registerObject(new Object(scopedEval(interpreterState, arguments[parameterIndex]), _gc_long)));
                            }
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
            ret = _gc_short->registerObject(new Object());
        }
        
        return ret;
    }

    void TinyClojure::CollectGarbage() {
        _gc_short->collectGarbage();
    }

#pragma mark -
#pragma mark Garbage Collector
    
    Object* GarbageCollector::registerObject(Object* object) {
        _objects.insert(object);
        return object;
    }

    void GarbageCollector::deleteObject(Object* object) {
        // Get object from _objects data structure
        std::set<Object*>::iterator objToDelete = _objects.find(object);
        // Erase from data structure
        _objects.erase(object);
        // Deallocate the memory dedicated to this object
        delete *objToDelete;
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
        for (std::set<Object*>::iterator it = _objects.begin(); it != _objects.end(); ++it) {
            delete *it;
        }
        _objects.clear();
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