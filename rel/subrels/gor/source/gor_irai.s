.global bIraiCmp1, bIraiCmp1Return
.global bIraiCmp2, bIraiCmp2Return
.global bIraiCmp3, bIraiCmp3Return
.global bIraiCmp4, bIraiCmp4Return
.global bIraiCmp5, bIraiCmp5Return
.global bIraiCmp6, bIraiCmp6Return
.global bIraiCmp7, bIraiCmp7Return
.global bIraiCmp8, bIraiCmp8Return
.global bIraiCmp9, bIraiCmp9Return

bIraiCmp1:
	stwu  %r1, -0x20(%r1)
	mflr  %r0
	stw   %r0, 0x08(%r1)
	stw   %r4, 0x10(%r1)
	stw   %r5, 0x14(%r1)
	stw   %r6, 0x18(%r1)
	li    %r3, 1711
	bl    swByteGet
	lwz   %r0, 0x08(%r1)
	mtlr  %r0
	lwz   %r4, 0x10(%r1)
	lwz   %r5, 0x14(%r1)
	lwz   %r6, 0x18(%r1)
	addi  %r1, %r1, 0x20
	cmpwi %r3, 8
bIraiCmp1Return:
	b 0

bIraiCmp2:
	stwu  %r1, -0x20(%r1)
	mflr  %r0
	stw   %r0, 0x08(%r1)
	stw   %r4, 0x10(%r1)
	stw   %r5, 0x14(%r1)
	stw   %r6, 0x18(%r1)
	li    %r3, 1713
	bl    swByteGet
	lwz   %r0, 0x08(%r1)
	mtlr  %r0
	lwz   %r4, 0x10(%r1)
	lwz   %r5, 0x14(%r1)
	lwz   %r6, 0x18(%r1)
	addi  %r1, %r1, 0x20
	cmpwi %r3, 11
bIraiCmp2Return:
	b 0

bIraiCmp3:
	stwu  %r1, -0x20(%r1)
	mflr  %r0
	stw   %r0, 0x08(%r1)
	stw   %r4, 0x10(%r1)
	stw   %r5, 0x14(%r1)
	stw   %r6, 0x18(%r1)
	li    %r3, 1703
	bl    swByteGet
	lwz   %r0, 0x08(%r1)
	mtlr  %r0
	lwz   %r4, 0x10(%r1)
	lwz   %r5, 0x14(%r1)
	lwz   %r6, 0x18(%r1)
	addi  %r1, %r1, 0x20
	cmpwi %r3, 20
bIraiCmp3Return:
	b 0

bIraiCmp4:
	stwu  %r1, -0x20(%r1)
	mflr  %r0
	stw   %r0, 0x08(%r1)
	stw   %r4, 0x10(%r1)
	stw   %r5, 0x14(%r1)
	stw   %r6, 0x18(%r1)
	li    %r3, 1715
	bl    swByteGet
	lwz   %r0, 0x08(%r1)
	mtlr  %r0
	lwz   %r4, 0x10(%r1)
	lwz   %r5, 0x14(%r1)
	lwz   %r6, 0x18(%r1)
	addi  %r1, %r1, 0x20
	cmpwi %r3, 8
bIraiCmp4Return:
	b 0

bIraiCmp5:
	stwu  %r1, -0x20(%r1)
	mflr  %r0
	stw   %r0, 0x08(%r1)
	stw   %r4, 0x10(%r1)
	stw   %r5, 0x14(%r1)
	stw   %r6, 0x18(%r1)
	li    %r3, 1717
	bl    swByteGet
	lwz   %r0, 0x08(%r1)
	mtlr  %r0
	lwz   %r4, 0x10(%r1)
	lwz   %r5, 0x14(%r1)
	lwz   %r6, 0x18(%r1)
	addi  %r1, %r1, 0x20
	cmpwi %r3, 18
bIraiCmp5Return:
	b 0

bIraiCmp6:
	stwu  %r1, -0x20(%r1)
	mflr  %r0
	stw   %r0, 0x08(%r1)
	stw   %r4, 0x10(%r1)
	stw   %r5, 0x14(%r1)
	stw   %r6, 0x18(%r1)
	li    %r3, 1706
	bl    swByteGet
	lwz   %r0, 0x08(%r1)
	mtlr  %r0
	lwz   %r4, 0x10(%r1)
	lwz   %r5, 0x14(%r1)
	lwz   %r6, 0x18(%r1)
	addi  %r1, %r1, 0x20
	cmpwi %r3, 1
bIraiCmp6Return:
	b 0

bIraiCmp7:
	stwu  %r1, -0x20(%r1)
	mflr  %r0
	stw   %r0, 0x08(%r1)
	stw   %r4, 0x10(%r1)
	stw   %r5, 0x14(%r1)
	stw   %r6, 0x18(%r1)
	li    %r3, 1706
	bl    swByteGet
	lwz   %r0, 0x08(%r1)
	mtlr  %r0
	lwz   %r4, 0x10(%r1)
	lwz   %r5, 0x14(%r1)
	lwz   %r6, 0x18(%r1)
	addi  %r1, %r1, 0x20
	cmpwi %r3, 43
bIraiCmp7Return:
	b 0

bIraiCmp8:
	stwu  %r1, -0x20(%r1)
	mflr  %r0
	stw   %r0, 0x08(%r1)
	stw   %r4, 0x10(%r1)
	stw   %r5, 0x14(%r1)
	stw   %r6, 0x18(%r1)
	li    %r3, 1707
	bl    swByteGet
	lwz   %r0, 0x08(%r1)
	mtlr  %r0
	lwz   %r4, 0x10(%r1)
	lwz   %r5, 0x14(%r1)
	lwz   %r6, 0x18(%r1)
	addi  %r1, %r1, 0x20
	cmpwi %r3, 16
bIraiCmp8Return:
	b 0

bIraiCmp9:
	stwu  %r1, -0x20(%r1)
	mflr  %r0
	stw   %r0, 0x08(%r1)
	stw   %r4, 0x10(%r1)
	stw   %r5, 0x14(%r1)
	stw   %r6, 0x18(%r1)
	li    %r3, 1708
	bl    swByteGet
	lwz   %r0, 0x08(%r1)
	mtlr  %r0
	lwz   %r4, 0x10(%r1)
	lwz   %r5, 0x14(%r1)
	lwz   %r6, 0x18(%r1)
	addi  %r1, %r1, 0x20
	cmpwi %r3, 17
bIraiCmp9Return:
	b 0

.global bIraiCompact, bIraiCompactReturn

bIraiCompact:
	li    %r5, 0
	li    %r6, 0
	li    %r0, 30
	mtctr %r0
bIraiCompactScan:
	addi  %r7, %r5, 0x8
	lwzx  %r7, %r31, %r7
	cmpwi %r7, 0
	beq   bIraiCompactSkip
	addi  %r3, %r6, 0x8
	stwx  %r7, %r31, %r3
	addi  %r6, %r6, 4
bIraiCompactSkip:
	addi  %r5, %r5, 4
	bdnz  bIraiCompactScan
	subf  %r0, %r6, %r5
	srwi. %r0, %r0, 2
	beq   bIraiCompactDone
	mtctr %r0
	li    %r7, 0
bIraiCompactZero:
	addi  %r3, %r6, 0x8
	stwx  %r7, %r31, %r3
	addi  %r6, %r6, 4
	bdnz  bIraiCompactZero
bIraiCompactDone:
	li    %r0, 0x1e
bIraiCompactReturn:
	b 0

.global bIraiRenderRedCheck, bIraiRenderRedCheckReturn

bIraiRenderRedCheck:
	lwzx  %r3, %r31, %r26
	addi  %r3, %r3, 1730
	bl swByteGet
	li %r0, 0x1
bIraiRenderRedCheckReturn:
	b 0

.global bIraiRenderGreyCheck, bIraiRenderGreyCheckReturn

bIraiRenderGreyCheck:
	addi  %r3, %r3, 1730
	bl swByteGet
bIraiRenderGreyCheckReturn:
	b 0

.global bIraiMainCompleteCheck, bIraiMainCompleteCheckReturn

bIraiMainCompleteCheck:
	addi  %r3, %r3, 1730
	bl swByteGet
bIraiMainCompleteCheckReturn:
	b 0
