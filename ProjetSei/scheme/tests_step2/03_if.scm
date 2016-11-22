;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Test if"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(if (0)(1)(0))
(if (1)(1)(0))
(if (#t)(1)(0))
(if ("lalala")(1)(0))
(if (())(1)(0))
(if (#\g)(1)(0))
(if (Symb)(1)(0))
(if (#f)(1)(0))
(if (0 < 1)(1)(0))
(if (1 < 0)(1)(0))