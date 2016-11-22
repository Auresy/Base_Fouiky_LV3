;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Test Boolean"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(boolean? #f)
(boolean? 15)
(boolean? #f #f #f #f)
(boolean? 15 #f #f)
(boolean? #f 15 #f)
(boolean? #f #f 15)