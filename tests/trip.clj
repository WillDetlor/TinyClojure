; basic "trip tests" for TinyClojure
; tests are designed to evaluate to 0

; define true to make verbose
(def verboseassert false)

(println "Working!")

; an assert statement
(defn assertzero [value label]
  (if (not= value 0)
    (print "Failure: " label)
    (if verboseassert
      (print "Success: " label))))

; basic test that defs remain within a scope
(def a 10)
(assertzero (- a 10) "1")
(do
    (def a 10)
    (assertzero (- a 10) "2"))

; make sure state is being captured - TODO this behaviour fails with real clojure right now, fix it
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

; test cond
(assertzero
  (cond
    (= 1 2)
      (do
        (print "this shouldn't happen")
        1)
    (= (+ 2 2) 0)
      (do
        (print "this shouldn't happen")
        1)
    (= 1 (- 2 1))
      0)
  "cond failure")
  
; test let statement
(assertzero
  (let [x 1 y (- x 1)] 12 y)
  "let failure")

; nth statement
(assertzero
  (nth [1 2 0 4] 2)
  "nth vector failure")
(assertzero
  (nth (list 1 2 0 4) 2)
  "nth list failure")
(assertzero
  (nth [1 2 3 4] 10 0)
  "nth vector failure")

(print "trip.clj finished")