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
    }
    
    TinyClojure::~TinyClojure() {
        delete _ioProxy;
    }
    
    
#pragma mark parser
    Object* TinyClojure::parse(std::string startText) {
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