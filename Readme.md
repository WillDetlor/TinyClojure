# TinyClojure

### What is TinyClojure

TinyClojure is a project to build a small, easily embeddable version of Clojure/ClojureScript. In many ways it is my attempt to create a Clojure equivalent of [TinyScheme](http://tinyscheme.sourceforge.net/home.html).

I started this project because I was looking for an embeddable Clojure for [Lisping](http://slidetocode.com/), an iPad Lisp development environment I develop.  Having used [TinyScheme](http://tinyscheme.sourceforge.net/home.html) for the scheme interpreter in lisping I was looking for a clojure version that is just as easily embeddable, but for Clojure. [ClojureC](https://github.com/schani/clojurec) is good, but the build process is complex, and there are external library dependencies.  These are not insurmountable, but they were sufficient to discourage me from using it.

TinyClojure will never have ClojureC's performance because my focus is on making it the easiest way to embed Clojure within any application.  There are just two files to compile into your code, no external dependencies, and the extension and embedding interface is as simple as it is possible to be.  I don't want TinyClojure to bloat, but I am happy for it to end up considerably larger than the almost impossibly slim TinyScheme.

A warning, these are very early days for TinyClojure, it is not feature complete, and it does not even carry a version number.  Feel free to use it for whatever you would to, but do not rely on it.

### How do I embed TinyClojure in my code?

Easily.

    tinyclojure::TinyClojure interpreter;
    tinyclojure::Object* code = interpreter.parse("(+ 1 2)");
    tinyclojure::Object* result = interpreter.eval(code);
    std::cout << result->integerValue();
    
All objects are garbage collected by the interpreter itself, so there is no need to delete them.


### Why?

It seems to me that Greenspun's tenth rule is correct.  To paraphrase "all sufficiently complex software will eventually contain an implementation of LISP".  I believe that

1. If you are going to have to embed anything, it should be easy.
2. That LISP should be Clojure.


### Exactly what language are you targeting?

Ostensibly Clojure, I would say ClojureScript, but when I last looked, ClojureScript lacked the (eval ) form, and for me, LISP without (eval ) is no LISP at all.  Right now, I'm looking to implement the Clojure language itself (without namespaces, imports or any of the Java interface), and its core library in [the Clojure.Core API](http://richhickey.github.com/clojure/clojure.core-api.html).

### TODO

If you want to get involved, get in touch with me at slidetocode at gmail dot com.  Forks and pull requests are a good thing.  The current TODO list is

Right now, the interpreter is "in progress", and it really just needs to get started, personal TODOs

###### Immediate tasks
* basic eval in clojure itself (needs read-string)
* validate argument type arrays
* write a trip.clj for current behaviours
* write (defn )
* should ensure all Object properties are indeed immutable

###### Minor
* swap all return NULLs for return nil object
* shouldn't need to reset the interpreter when an extension function is added
* work on the command line interface

###### Major
* variable length argument lists to fn (the &)
* full numeric stack.  Only cater for integers and floats right now, need fractions
* Test suite.  This is sadly lacking right now.
* refactoring.  C++ is not my "first language" in the programming world, so any refactors to make it more idiomatic would be appreciated.
* Garbage collector.  Right now there is no garbage collector to speak of.  Objects collect until the interpreter destructs, then they are deleted.
* Parser rewrite.  I converted the parser from the tolerant parser used in Lisping.  It is neither elegant, nor an appropriate design.  I would like to replace it with something more elegant once this interpreter is up and running.
* Implement all the Clojure.Core functions.
* A more efficient structure.  TinyClojure converts code into Clojure data, then recursively evalutes it.  This eats stack space and it is not efficient either in terms of speed or memory.  A bytecode version would increase the size of the source code somewhat, but it would greatly increase efficiency.


### Coding conventions

Roughly, my coding conventions are

* Keep everything in the "tinyclojure" namespace please.
* Datatypes start with a capital, member variables with an underscore.
* Indent by four spaces, no tabs.
* No macros (C++ of course).
* Document all interfaces with Doxygen style comments.
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


### Contact

If you want to contact me, email me at slidetocode at gmail dot com


### License

Tiny clojure is licensed under the standard MIT license so that it can be used anywhere, for anything.  Nevertheless, pull requests would be greatly appreciated