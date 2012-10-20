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
#pragma mark TinyClojure
    
    TinyClojure::TinyClojure() {
        _ioProxy = new IOProxy();
        _gc = new GarbageCollector();
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
    
    Object* TinyClojure::recursiveParse(ParserState& parseState) {
        parseState.skipNewLinesAndWhitespace();
        
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
            NSMutableString *stringbuf = [NSMutableString string];
            
            BOOL escapeNextChar = NO;
            
            while (state->position < state.parseString.length) {
                const unichar currentChar = [state.parseString characterAtIndex:state->position];
                
                /// TODO check for escapes
                [stringbuf appendFormat:@"%C",currentChar];
                (state->position)++;
                
                if (escapeNextChar) {
                    escapeNextChar = NO;
                } else {
                    if (currentChar == '\\') {
                        escapeNextChar = YES;
                    } else if (currentChar=='"' && stringbuf.length>1) {
                        // in the case of regexes, add the hash to the string
                        if (stringType == stringTypeRegex)
                            stringbuf = [NSString stringWithFormat:@"#%@",stringbuf];
                        
                        // end of the string
                        TSTextElement *el = [TSTextElement elementWithLayoutOptions:layoutOptions];
                        el.text = stringbuf;
                        return el;
                    }
                }
            }
            
            // ran out of characters.  It is helpful to close the qoute and return what is left as a string
            [stringbuf appendString:@"\""];
            if (stringType == stringTypeRegex)
                stringbuf = [NSString stringWithFormat:@"#%@",stringbuf];
            TSTextElement *el = [TSTextElement elementWithLayoutOptions:layoutOptions];
            el.text = stringbuf;
            return el;
        } else if (startChar == '(' || sexpType == sexpTypeHashSet) {
            unichar closeChar = ')';
            if (sexpType == sexpTypeHashSet) closeChar = '}';
            
            /*
             * start an S-Expression
             *
             * We are looking for
             * * Open parenthesis (found)
             * * terms (recurse this function)
             * * Close parenthesis
             */
            NSMutableArray *elements = [NSMutableArray array];
            
            // advance from the parenthesis
            (state->position)++;
            
            while (YES) {
                // move on
                [self skipSet:separatorSet state:state];
                
                if (state->position>=state.parseString.length) {
                    // nothing left, return the s expression so far
                    break;
                }
                
                if ([state.parseString characterAtIndex:state->position]==closeChar) {
                    // advance past the ) and end the S expression
                    (state->position)++;
                    
                    break;
                }
                
                TSElement *el = [self parse:state withLayoutOptions:layoutOptions];
                if (el)
                    [elements addObject:el];
                else {
                    // nothing left, return the s expression so far
                    break;
                }
            }
            
            switch (sexpType) {
                case sexpTypeNormal:
                    return [TSClojureSExpression SExpressionFromElements:elements
                                                       withLayoutOptions:layoutOptions
                                                                  source:self];
                    break;
                    
                case sexpTypeListLiteral:
                    return [TSClojureListLiteral SExpressionFromElements:elements
                                                       withLayoutOptions:layoutOptions];
                    break;
                    
                case sexpTypeLambdaShorthand:
                    return [TSClojureLambdaShorthand SExpressionFromElements:elements
                                                           withLayoutOptions:layoutOptions
                                                                      source:self];
                    break;
                    
                case sexpTypeHashSet:
                    return [TSClojureHashSet SExpressionFromElements:elements
                                                   withLayoutOptions:layoutOptions];
                    break;
            }
            
            return nil;
        } else if (startChar == '[') {
            /*
             * start a Clojure vector
             *
             * We are looking for
             * * Open parenthesis (found)
             * * terms (recurse this function)
             * * Close parenthesis
             */
            NSMutableArray *elements = [NSMutableArray array];
            
            // advance from the parenthesis
            (state->position)++;
            
            while (YES) {
                // move on
                [self skipSet:separatorSet state:state];
                
                if (state->position>=state.parseString.length) {
                    // nothing left, return the vector so far
                    return [TSClojureVector SExpressionFromElements:elements
                                                  withLayoutOptions:layoutOptions];
                }
                
                if ([state.parseString characterAtIndex:state->position]==']') {
                    // advance past the ] and end the vector
                    (state->position)++;
                    
                    return [TSClojureVector SExpressionFromElements:elements
                                                  withLayoutOptions:layoutOptions];
                }
                
                TSElement *el = [self parse:state withLayoutOptions:layoutOptions];
                if (el)
                    [elements addObject:el];
                else {
                    // nothing left, return the s expression so far
                    return [TSClojureVector SExpressionFromElements:elements
                                                  withLayoutOptions:layoutOptions];
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
            NSMutableArray *elements = [NSMutableArray array];
            
            // advance from the parenthesis
            (state->position)++;
            
            while (YES) {
                // move on
                [self skipSet:separatorSet state:state];
                
                if (state->position>=state.parseString.length) {
                    // nothing left, return the vector so far
                    return [TSClojureMap SExpressionFromElements:elements
                                               withLayoutOptions:layoutOptions];
                }
                
                if ([state.parseString characterAtIndex:state->position]=='}') {
                    // advance past the ] and end the vector
                    (state->position)++;
                    
                    return [TSClojureMap SExpressionFromElements:elements
                                               withLayoutOptions:layoutOptions];
                }
                
                TSElement *el = [self parse:state withLayoutOptions:layoutOptions];
                if (el)
                    [elements addObject:el];
                else {
                    // nothing left, return the s expression so far
                    return [TSClojureMap SExpressionFromElements:elements
                                               withLayoutOptions:layoutOptions];
                }
            }
        } else if (startChar == ';') {
            /// start a comment, run until the end of the line
            NSMutableString *comment = [NSMutableString string];
            
            while (state->position < state.parseString.length) {
                const unichar currentChar = [state.parseString characterAtIndex:state->position];
                
                BOOL commentChar = NO;
                
                if (![newlineSet characterIsMember:currentChar]) {
                    [comment appendFormat:@"%C",currentChar];
                    commentChar = YES;
                    (state->position)++;
                }
                
                if (!commentChar || state->position>=state.parseString.length) {
                    // end of the identifier
                    return [TSCommentTextElement elementWithLayoutOptions:layoutOptions andText:comment];
                }
            }
        } else if (startChar == '`' || startChar == '\'') {
            // start collecting a symbol
            // TODO no real difference between symbols, identifiers and numbers
            NSMutableString *symbol = [NSMutableString string];
            
            while (state->position < state.parseString.length) {
                const unichar   currentChar = [state.parseString characterAtIndex:state->position];
                
                unichar peekChar = 0,
                peekPeekChar = 0;
                if (state->position<state.parseString.length-1)
                    peekChar = [state.parseString characterAtIndex:state->position+1];
                if (state->position<state.parseString.length-2)
                    peekPeekChar = [state.parseString characterAtIndex:state->position+2];
                
                BOOL identifierChar = NO;
                
                if (![excludeSet characterIsMember:currentChar]
                    || (symbol.length == 0)) {
                    [symbol appendFormat:@"%C",currentChar];
                    identifierChar = YES;
                    (state->position)++;
                }
                
                if (!identifierChar || state->position>=state.parseString.length || (peekChar=='#' && peekPeekChar=='"')) {
                    // end of the identifier
                    TSTextElement *el = [TSTextElement elementWithLayoutOptions:layoutOptions];
                    el.text = symbol;
                    return el;
                }
            }
        } else if (![excludeSet characterIsMember:startChar]) {
            // start an identifier
            NSMutableString *identifier = [NSMutableString string];
            
            while (state->position < state.parseString.length) {
                const unichar currentChar = [state.parseString characterAtIndex:state->position];
                
                unichar peekChar = 0,
                peekPeekChar = 0;
                if (state->position<state.parseString.length-1)
                    peekChar = [state.parseString characterAtIndex:state->position+1];
                if (state->position<state.parseString.length-2)
                    peekPeekChar = [state.parseString characterAtIndex:state->position+2];
                
                
                BOOL identifierChar = NO;
                
                if (![excludeSet characterIsMember:currentChar]
                    || ((startChar=='#') && (currentChar==';') && (state->position == startPosition+1))) {    // case to deal with inline comments
                    [identifier appendFormat:@"%C",currentChar];
                    identifierChar = YES;
                    (state->position)++;
                }
                
                if (!identifierChar || state->position>=state.parseString.length || (peekChar=='#' && peekPeekChar=='"')) {
                    if ([identifier rangeOfString:@"#;"].location==0) {
                        // inline comment element
                        return [TSCommentTextElement elementWithLayoutOptions:layoutOptions andText:identifier];
                    } else {
                        // standard text element
                        return [TSTextElement elementWithLayoutOptions:layoutOptions andText:identifier];
                    }
                }            
            }
        }
        
        return nil;
    }
        
        return NULL;
    }
    
#pragma mark evaluator
    
    Object* TinyClojure::eval(Object* code) {
        return NULL;
    }
    
#pragma mark -
#pragma mark Object



#pragma mark -
#pragma mark Garbage Collector
    
    void GarbageCollector::registerObject(Object* object) {
        _objects.insert(object);
    }
    
    GarbageCollector::GarbageCollector() {
    }
    
    GarbageCollector::~GarbageCollector() {
        for (std::set<Object*>::iterator it = _objects.begin(); it != _objects.end(); ++it)
            delete *it;
    }
}