.section .text

.align 2
.global trap_vector

trap_vector:
    call trap_handler
    csrr t0, mepc
    addi t0, t0, 4
    csrw mepc, t0
    mret
