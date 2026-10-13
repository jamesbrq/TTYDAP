.section .text.SmartAllocFailure,"ax",@progbits
.global SmartAllocFailure
.global SmartAllocFailureReturn
SmartAllocFailure:
    stwu %r1,-32(%r1)
    mflr %r0
    stw %r0,36(%r1)
    mr %r3,%r31
    bl SmartReturnFailedRecord
    lwz %r0,36(%r1)
    mtlr %r0
    addi %r1,%r1,32
    li %r3,0
SmartAllocFailureReturn:
    b 0
