;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Test and..."
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(and 4 #t)
(and 4 #f)
(and abc #t)
(and abc #f)
(and "bli" #t)
(and "bli" #f)
(and #\a #t)
(and #\a #f)
(and () #t)
(and () #f)
(and #f #t)
(and #f #f)
(and #t #t)
(and #t #f)