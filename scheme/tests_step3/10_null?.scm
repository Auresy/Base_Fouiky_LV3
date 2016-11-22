;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Test null?..."
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(null? ())
(null? 15)
(null? () () () ())
(null? 15 () ())
(null? () 15 ())
(null? ()() 15)
