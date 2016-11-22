;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Test quote"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(quote 45)
'45
(quote #t)
'#t
(quote #\c)
'#\c
(quote "bla bli blo")
(quote ())
'()
(quote Symbol)
'Symbol
(quote (45 12))
'(45 12)
(quote ((45 12) ("bibou" () #t #\p symb)))
'((45 12) ("bibou" () #t #\p symb))