;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Begin"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(begin (define a 12) (set! a ( or #f #t ) ) 3 a )
(begin 1 2 3)
(begin "bla" " bli " "blo")
(begin (if #f 1 (define a 12) ) (+ a 1) a )
