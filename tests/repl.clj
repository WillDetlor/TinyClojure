(defn repl []
  (eval (read-string (read-line)))
  (repl))

(repl)
