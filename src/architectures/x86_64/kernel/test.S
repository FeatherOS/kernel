;.section .text
;.align 4

;.global testing
;.type testing, @function
;.global testing
;.type testing, @function
;testing:
    ;ljmp a
;a:
    ;ljmp testing

global testing
testing:
    jmp $