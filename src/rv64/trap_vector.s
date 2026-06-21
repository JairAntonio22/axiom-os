.section .text

.align 2
.global trap_vector

trap_vector:
    call trap_handler
    mret
