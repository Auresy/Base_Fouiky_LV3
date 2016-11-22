;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Test setcdr"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(set-cdr! (12) (12))
(set-cdr! (12 3) (12 5 6 7))
(set-cdr! (12 1 2 4) (2))