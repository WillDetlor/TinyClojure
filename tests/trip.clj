; basic "trip tests" for TinyClojure
; ideally everything should evaluate to 0 unless otherwise noted

; test scopes
(print "1. Functions and scopes")

; basic test that defs remain within a scope
(def a 10)
print (- a 10)
(do
    (def a 10)
    print (- a 10))

; make sure state is being captured
(def con 12)
(def foo (fn [x] (- x con)))
(def con 13)
(print (foo 12))

; basic test of a closure
(def factory
    (fn [x]
        (fn [y]
            (- x y))))
(def foo (factory 12))
(print (foo 12))
