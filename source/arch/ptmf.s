.text

.global __ptmf_test
__ptmf_test:
    lwz r5, 0(r3)
    lwz r6, 4(r3)
    lwz r7, 8(r3)
    li r3, 1
    cmpwi r5, 0
    cmpwi cr6, r6, 0
    cmpwi cr7, r7, 0
    bnelr
    bnelr cr6
    bnelr cr7
    li r3, 0
    blr

.global __ptmf_cmpr
__ptmf_cmpr:
    lwz r5, 0(r3)
    lwz r6, 0(r4)
    lwz r7, 4(r3)
    lwz r8, 4(r4)
    lwz r9, 8(r3)
    lwz r10, 8(r4)
    li r3, 1
    cmpw r5, r6
    cmpw cr6, r7, r8
    cmpw cr7, r9, r10
    bnelr
    bnelr cr6
    bnelr cr7
    li r3, 0
    blr

.global __ptmf_scall
__ptmf_scall:
    lwz r0, 0(r12)
    lwz r11, 4(r12)
    lwz r12, 8(r12)
    add r3, r3, r0
    cmpwi r11, 0
    blt moveToCounterReg
    lwzx r12, r3, r12
    lwzx r12, r12, r11

moveToCounterReg:
    mtctr r12
    bctr