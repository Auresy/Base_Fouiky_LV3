;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Test or..."
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(or 4 #t)
(or 4 #f)
(or abc #t)
(or abc #f)
(or "bli" #t)
(or "bli" #f)
(or #\a #t)
(or #\a #f)
(or () #t)
(or () #f)
(or #f #t)
(or #f #f)
(or #t #t)
(or #t #f)