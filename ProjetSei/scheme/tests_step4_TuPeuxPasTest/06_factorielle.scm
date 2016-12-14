;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Factorielle"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define (fact n) (if (< n 2) 1 (* n (fact (- n 1) ) )))
(fact 5)
