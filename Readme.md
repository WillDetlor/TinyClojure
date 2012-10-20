# TinyClojure

### What is TinyClojure

TinyClojure is a project to build a minute, easily embeddable version of ClojureScript.  Essentially I'm building [TinyScheme](), just with Clojure.  For TinyClojure small size, hackability and ease of embedding will always take precedence over efficiency.  For a embeddable, high performance ClojureScript interpreter in C please see []().

I started this project because I was lookin for an embeddable clojure for [Lisping](), and iPad Lisp development environment.  You can see it in action there.


### How do I embed TinyClojure in my code?

Easily.

    tinyclojure::TinyClojure interpreter;
    tinyclojure::Object* code = interpreter.parse("(+ 1 2)");
    interpreter.eval(code);
    
All objects are garbage collected by the interpreter itself, so there is no need to delete them.


### Why?

Greenspun's tenth rule, "all sufficiently complex software will eventually contain an implementation of LISP", has been proven again.  I believe that

1. If you are going to have to do it, it may as well be easy.
2. That LISP should be Clojure.


### Exactly what language are you targeting?

Ostensibly ClojureScript, but ClojureScript


### Coding conventions

Roughly, my coding conventions are

* Keep everything in the "tinyclojure" namespace please.
* Datatypes start with a capital, member variables with an underscore.
* Indent by four spaces, no tabs.
* No macros (C++ of course).
* Document all interfaces with Doxygen style comments.
* All variable and method names must be descriptive.  No one letter variables please.  Comment code when necessary, but if you can clarify the operation of your code through the method and variables names instead, that is better.

Less defined principles are

* Order of precedence
    * Hackability.  No extension language is at all useful unless it is easy to read, fix and extend.
    * Small compiled size.  Add as many braces and comments as you like, but this must compile to be small
    * Ease of embedding.  One .h, one .cpp and four lines of code should be all you need to embed this in your code.
    * Startup speed.  In general I'm not sorried about speed, but an interpreter cannot be regarded as lightweight if takes a long time to construct and initialise.
    * Execution speed.  I'm willing to relax the small size rule a little for large performance gains, but this interpreter is only meant for small scripts.  Execution speed just isn't that important.
* If you want to add functionality that violates the golden rule, then as long as it can be turned on and off with an #ifdef then that is fine.


### License

Tiny clojure is licensed under the standard MIT license so that it can be used anywhere, for anything.

