;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="Test !set..."
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define x 165)
x
(!set x 561)
x
(define symb adc)
symb
(!set symb cda)
symb
(define y "bli")
y
(!set y "coucou")
y
(define z #t)
z
(!set z #f)
z
(define a #\o)
a
(!set a #\d)
a
(define k 561)
k
(!set k ())
k
