; basic "trip tests" for TinyClojure
; ideally everything should evaluate to 0 unless otherwise noted

; an assert statement
(defn assertzero [value label]
  (if (not= value 0)
    (print "Failure: " label)))

; basic test that defs remain within a scope
(def a 10)
(assertzero (- a 10) "1")
(do
    (def a 10)
    (assertzero (- a 10) "2"))

; make sure state is being captured
(def con 12)
(def foo (fn [x] (- x con)))
(def con 13)
(assertzero (foo 12) "3")

; basic test of a closure
(def factory
    (fn [x]
        (fn [y]
            (- x y))))
(def foo (factory 12))
(assertzero (foo 12) "4")