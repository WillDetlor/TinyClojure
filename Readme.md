# TinyClojure

### What is TinyClojure

TinyClojure is a project to build a small, easily embeddable version of Clojure/ClojureScript in portable C++. In many ways it is my attempt to create a Clojure equivalent of [TinyScheme](http://tinyscheme.sourceforge.net/home.html).

I started this project because I was looking for an embeddable Clojure for [Lisping](http://slidetocode.com/), an iPad Lisp development environment I develop.  Having used the excellent [TinyScheme](http://tinyscheme.sourceforge.net/home.html) for the Scheme interpreter in Lisping I wanted looking for a clojure version that is just as easily embeddable. [ClojureC](https://github.com/schani/clojurec) is good, but the build process is complex, and there are external library dependencies.  These are not insurmountable, but they were sufficient to discourage me from using it.

The focus with TinyClojure's development is to make it the easiest way to embed Clojure within any application.  Tiny Clojure consists of one header file, one source file, no external dependencies, and the extension and embedding interface is as simple as it is possible to be.  All forms are subclasses of the ExtensionFunction class, whether they are builtin control statements like (if ), (cond ), (do ) or custom extension functions.  This structure keeps TinyClojure's core minimal, so development is easy and contained.

A warning, these are very early days for TinyClojure, the core of the interpreter works well and I have implemented the most basic functions for TinyClojure to function as a language (simple io, simple arithmetic, the most basic control structures).  I am now working on filling out Clojure's Core API.  Feel free to use it for whatever you would to, but do not rely on it.

Obviously, this is an open source project and if you want to get involved, fork it and get going.  You can get in touch with me at <slidetocode@gmail.com>.  The real priority for now is to fill out the standard library so that TinyClojure is usable as a language, then I would like to work on more complex issues such as threading.

### How do I embed TinyClojure in my code?

Easily.

    tinyclojure::TinyClojure interpreter;
    tinyclojure::Object* code = interpreter.parse("(+ 1 2)");
    tinyclojure::Object* result = interpreter.eval(code);
    std::cout << result->integerValue();
    
All objects are garbage collected by the interpreter itself, so there is no need to delete them.


### Exactly what language are you targeting?

Ostensibly Clojure, I would say ClojureScript, but when I last looked, ClojureScript lacked the (eval ) form, and for me, LISP without (eval ) is no LISP at all.  Right now, I'm looking to implement the Clojure language itself (without namespaces, imports or any of the Java interface), and its core library in [the Clojure.Core API](http://richhickey.github.com/clojure/clojure.core-api.html).

### TODO

This project is very young, with a very vague and incomplete TODO list.

###### Immediate tasks

* implement an iterable interface, and make forms such as (nth ) work on both vectors and lists

###### Forms to implement

* recurse structures
* map
* many more, import and prioritise the Clojure.core AOI

###### Minor

* fix the bugs with lists in stringRepresentation()
* sort :else case in a (cond ) statement
* swap all return NULLs for return nil object
* shouldn't need to reset the interpreter when an extension function is added
* work on the command line interface
* correct the (print ) form to 
* parse \n, \r, \t, etc. correctly in strings
* parse comments
* variable length argument lists to fn (the &)

###### Major

* implement all datastructures (the various hashmaps, etc.)
* full numeric stack.  Only cater for integers and floats right now, need fractions
* Test suite.  This is sadly lacking right now.  test.clj is the beginnings of my testing
* refactoring.  C++ is not my "first language" in the programming world, so any refactors to make it more idiomatic would be appreciated.
* Garbage collector.  Right now there is no garbage collector to speak of.  Objects collect until the interpreter destructs, then they are deleted.
* Parser rewrite.  I converted the parser from the tolerant parser used in Lisping.  It is neither elegant, nor an appropriate design.  I would like to replace it with something more elegant once this interpreter is up and running.
* Implement all the Clojure.Core functions.
* Better error reporting
* Macros

###### Oneday

* A more efficient structure.  TinyClojure converts code into Clojure data, then recursively evalutes it.  This eats stack space and it is not efficient either in terms of speed or memory.  A bytecode structure would increase the size of the source code somewhat, but it would greatly increase efficiency.

### Coding conventions

Roughly, my coding conventions are

* Keep everything in the "tinyclojure" namespace please.
* Datatypes start with a capital, member variables with an underscore.
* Indent by four spaces, no tabs.
* No macros (I'm talking about the C++ not the Lisp of course).
* Document all interfaces with Doxygen style comments.
* Add each feature to trip.clj (in as diabolic a manner as you please)
* All variable and method names must be descriptive.  No one letter variables please.  Comment code when necessary, but if you can clarify the operation of your code through the method and variables names instead, that is better.
* Git
    * If possible no broken commits, it is a disaster when git bisecting.

Less defined principles are

* Order of precedence
    * Hackability.  No extension language is at all useful unless it is easy to read, fix and extend.
    * Ease of embedding.  One .h, one .cpp and four lines of code should be all you need to embed this in your code.
    * Small compiled size.  I'm not aiming to match TinyScheme's minute footprint, but noone likes big bulky software.
    * Startup speed.  In general I'm not worried about speed, but an interpreter cannot be regarded as lightweight if takes a long time to construct and initialise.
    * Execution speed.  I'm willing to relax the small size rule a little for large performance gains, but this interpreter is only meant for small scripts.  Execution speed just isn't that important.
* If you want to add functionality that violates the golden rule, then as long as it can be turned on and off with an #ifdef then that is fine.


### Hacking TinyClojure

Tiny Clojure is designed to be as hackable as possible, and all interfaces are documented with Doxygen to make this as accessible as possible.  However here are a few bullet points to help you before you dive into the source.

* `Object` is the fundamental dynamic type in TinyClojure.  All code, data and functions (whether closure or builtins) are instances of this type.  The convention is that when creating any object, it must be registered with the garbage collector.
* Parsing and Evaluation are handled by The `TinyClojure` object.  Pass a string to the parse function and it will return the parsed source code as a data structure.  To execute this code, call eval on that data.  Internally there is both a scoped and an unscoped eval function. Scoped eval is a wrapper around unscoped eval that creates a new scope before evaluating the code.  By default use this as it will
* Interpreter scope objects represent the current "scope", these are simply wrappers around a dictionary of defined names and values
* subclasses of ExtensionFunction are responsible for all builtin forms, and they are the mechanism for extending the language.  To add a form, create a subclass of ExtensionFunction and register it with the TinyClojure object
* The number class is a type wrapping Clojure's "numeric tower"

### Contact

If you want to contact me, email me at slidetocode at gmail dot com


### License

Tiny clojure is licensed under the standard MIT license so that it can be used anywhere, for anything, as long as you keep the copyright notice.  Nevertheless, pull requests would be greatly appreciated if you improve TinyClojure.