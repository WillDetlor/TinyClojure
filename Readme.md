# TinyClojure

### What is TinyClojure

TinyClojure is a project to build a minute, easily embeddable version of Clojure/ClojureScript, essentially [TinyScheme](http://tinyscheme.sourceforge.net/home.html), but for Clojure.  For TinyClojure small size, hackability and ease of embedding will always take precedence over efficiency.

I started this project because I was looking for an embeddable Clojure for [Lisping](http://slidetocode.com/), an iPad Lisp development environment I develop.  I wanted the functionality of [ClojureC](https://github.com/schani/clojurec), but without the external library dependencies and the complex build process, in other words a Clojure version of [TinyScheme](http://tinyscheme.sourceforge.net/home.html).  

It is very early days still, and this interpreter is not usable yet.  It shouldn't be long though.


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

* implement floats
* create new scopes when necessary


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