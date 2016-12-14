;;;; En tete pour script de test ;;;;;;;;;;
; TEST_RETURN_CODE=PASS
; TEST_COMMENT="map"
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(map - ( quote (1 2 3) ) )
(map + (quote (1 2 3) ) (quote (1 2 -5)) )