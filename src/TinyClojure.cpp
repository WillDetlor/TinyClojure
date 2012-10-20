//
//  TinyClojure.cpp
//  TinyClojure
//
//  Created by Duncan Steele on 20/10/2012.
//  Copyright (c) 2012 Slide to code. All rights reserved.
//

#include "TinyClojure.h"

namespace tinyclojure {

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
            
            case kObjectTypeCons:
                // it isn't our business deleting "unused" objects, that is for the GC
            case kObjectTypeNil:
                // nothing need be done
                break;
        }
    }
    
    Object::Object(Object *left, Object *right) {
        _type = kObjectTypeCons;
        pointer.consValue.left = left;
        pointer.consValue.right = right;
    }
        
    std::string& Object::stringValue() {
        return *pointer.stringValue;
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
        
        for (char excludeChar = 1; excludeChar<32; ++excludeChar) {
            _excludeSet.append(&excludeChar, 1);
        }
        _excludeSet.append("\"()[]{}';` ");
    }
    
    TinyClojure::~TinyClojure() {
        delete _ioProxy;
        delete _gc;
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
                
                if (escapeNextChar) {
                    escapeNextChar = false;
                } else {
                    if (currentChar == '\\') {
                        escapeNextChar = true;
                    } else if (currentChar=='"' && stringbuf.length()>1) {
                        // in the case of regexes, add the hash to the string
                        if (stringType == stringTypeRegex) {
                            stringbuf.insert(0, "#");
                        }
                        
                        // end of the string
                        Object *element = new Object(stringbuf);
                        _gc->registerObject(element);
                        return element;
                    }
                }
            }
            
            ParserError error(parseState, std::string("Ran out of characters when parsing a string"));
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
                    ParserError error(parseState, "Ran out of characters when building an S Expression");
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
                    ParserError error(parseState, "Ran out of characters when building a vector");
                    throw error;
                }
                
                if (parseState.currentChar()==']') {
                    // advance past the ] and end the vector
                    ++parseState.position;
                    
                    return listObject(elements);
                }
                
                Object *element = recursiveParse(parseState);
                if (element) {
                    elements.push_back(element);
                } else {
                    ParserError error(parseState, "Ran out of characters when building a vector");
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
                    ParserError error(parseState, "Ran out of characters when building a map");
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
                    ParserError error(parseState, "Ran out of characters when building a map");
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
                        // standard text, ie a symbol
                        
                        // TODO test for number
                        
                        return _gc->registerObject(new Object(identifier, true));
                    }
                }            
            }
        }
                
        return NULL;
    }
    
#pragma mark evaluator
    
    Object* TinyClojure::eval(Object* code) {
        return NULL;
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