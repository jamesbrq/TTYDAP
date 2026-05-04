.set IRAI_GSW_GROUP_1, 1711
.set IRAI_GSW_GROUP_2, 1713
.set IRAI_GSW_GROUP_3, 1703
.set IRAI_GSW_GROUP_4, 1715
.set IRAI_GSW_GROUP_5, 1717
.set IRAI_GSW_GROUP_6, 1706
.set IRAI_GSW_GROUP_7, 1706
.set IRAI_GSW_GROUP_8, 1707
.set IRAI_GSW_GROUP_9, 1708

.set IRAI_THRESH_GROUP_1, 8
.set IRAI_THRESH_GROUP_2, 11
.set IRAI_THRESH_GROUP_3, 20
.set IRAI_THRESH_GROUP_4, 8
.set IRAI_THRESH_GROUP_5, 18
.set IRAI_THRESH_GROUP_6, 1
.set IRAI_THRESH_GROUP_7, 43
.set IRAI_THRESH_GROUP_8, 16
.set IRAI_THRESH_GROUP_9, 17

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
	li    %r3, IRAI_GSW_GROUP_1
	bl    swByteGet
	lwz   %r0, 0x08(%r1)
	mtlr  %r0
	lwz   %r4, 0x10(%r1)
	lwz   %r5, 0x14(%r1)
	lwz   %r6, 0x18(%r1)
	addi  %r1, %r1, 0x20
	cmpwi %r3, IRAI_THRESH_GROUP_1
bIraiCmp1Return:
	b 0

bIraiCmp2:
	stwu  %r1, -0x20(%r1)
	mflr  %r0
	stw   %r0, 0x08(%r1)
	stw   %r4, 0x10(%r1)
	stw   %r5, 0x14(%r1)
	stw   %r6, 0x18(%r1)
	li    %r3, IRAI_GSW_GROUP_2
	bl    swByteGet
	lwz   %r0, 0x08(%r1)
	mtlr  %r0
	lwz   %r4, 0x10(%r1)
	lwz   %r5, 0x14(%r1)
	lwz   %r6, 0x18(%r1)
	addi  %r1, %r1, 0x20
	cmpwi %r3, IRAI_THRESH_GROUP_2
bIraiCmp2Return:
	b 0

bIraiCmp3:
	stwu  %r1, -0x20(%r1)
	mflr  %r0
	stw   %r0, 0x08(%r1)
	stw   %r4, 0x10(%r1)
	stw   %r5, 0x14(%r1)
	stw   %r6, 0x18(%r1)
	li    %r3, IRAI_GSW_GROUP_3
	bl    swByteGet
	lwz   %r0, 0x08(%r1)
	mtlr  %r0
	lwz   %r4, 0x10(%r1)
	lwz   %r5, 0x14(%r1)
	lwz   %r6, 0x18(%r1)
	addi  %r1, %r1, 0x20
	cmpwi %r3, IRAI_THRESH_GROUP_3
bIraiCmp3Return:
	b 0

bIraiCmp4:
	stwu  %r1, -0x20(%r1)
	mflr  %r0
	stw   %r0, 0x08(%r1)
	stw   %r4, 0x10(%r1)
	stw   %r5, 0x14(%r1)
	stw   %r6, 0x18(%r1)
	li    %r3, IRAI_GSW_GROUP_4
	bl    swByteGet
	lwz   %r0, 0x08(%r1)
	mtlr  %r0
	lwz   %r4, 0x10(%r1)
	lwz   %r5, 0x14(%r1)
	lwz   %r6, 0x18(%r1)
	addi  %r1, %r1, 0x20
	cmpwi %r3, IRAI_THRESH_GROUP_4
bIraiCmp4Return:
	b 0

bIraiCmp5:
	stwu  %r1, -0x20(%r1)
	mflr  %r0
	stw   %r0, 0x08(%r1)
	stw   %r4, 0x10(%r1)
	stw   %r5, 0x14(%r1)
	stw   %r6, 0x18(%r1)
	li    %r3, IRAI_GSW_GROUP_5
	bl    swByteGet
	lwz   %r0, 0x08(%r1)
	mtlr  %r0
	lwz   %r4, 0x10(%r1)
	lwz   %r5, 0x14(%r1)
	lwz   %r6, 0x18(%r1)
	addi  %r1, %r1, 0x20
	cmpwi %r3, IRAI_THRESH_GROUP_5
bIraiCmp5Return:
	b 0

bIraiCmp6:
	stwu  %r1, -0x20(%r1)
	mflr  %r0
	stw   %r0, 0x08(%r1)
	stw   %r4, 0x10(%r1)
	stw   %r5, 0x14(%r1)
	stw   %r6, 0x18(%r1)
	li    %r3, IRAI_GSW_GROUP_6
	bl    swByteGet
	lwz   %r0, 0x08(%r1)
	mtlr  %r0
	lwz   %r4, 0x10(%r1)
	lwz   %r5, 0x14(%r1)
	lwz   %r6, 0x18(%r1)
	addi  %r1, %r1, 0x20
	cmpwi %r3, IRAI_THRESH_GROUP_6
bIraiCmp6Return:
	b 0

bIraiCmp7:
	stwu  %r1, -0x20(%r1)
	mflr  %r0
	stw   %r0, 0x08(%r1)
	stw   %r4, 0x10(%r1)
	stw   %r5, 0x14(%r1)
	stw   %r6, 0x18(%r1)
	li    %r3, IRAI_GSW_GROUP_7
	bl    swByteGet
	lwz   %r0, 0x08(%r1)
	mtlr  %r0
	lwz   %r4, 0x10(%r1)
	lwz   %r5, 0x14(%r1)
	lwz   %r6, 0x18(%r1)
	addi  %r1, %r1, 0x20
	cmpwi %r3, IRAI_THRESH_GROUP_7
bIraiCmp7Return:
	b 0

bIraiCmp8:
	stwu  %r1, -0x20(%r1)
	mflr  %r0
	stw   %r0, 0x08(%r1)
	stw   %r4, 0x10(%r1)
	stw   %r5, 0x14(%r1)
	stw   %r6, 0x18(%r1)
	li    %r3, IRAI_GSW_GROUP_8
	bl    swByteGet
	lwz   %r0, 0x08(%r1)
	mtlr  %r0
	lwz   %r4, 0x10(%r1)
	lwz   %r5, 0x14(%r1)
	lwz   %r6, 0x18(%r1)
	addi  %r1, %r1, 0x20
	cmpwi %r3, IRAI_THRESH_GROUP_8
bIraiCmp8Return:
	b 0

bIraiCmp9:
	stwu  %r1, -0x20(%r1)
	mflr  %r0
	stw   %r0, 0x08(%r1)
	stw   %r4, 0x10(%r1)
	stw   %r5, 0x14(%r1)
	stw   %r6, 0x18(%r1)
	li    %r3, IRAI_GSW_GROUP_9
	bl    swByteGet
	lwz   %r0, 0x08(%r1)
	mtlr  %r0
	lwz   %r4, 0x10(%r1)
	lwz   %r5, 0x14(%r1)
	lwz   %r6, 0x18(%r1)
	addi  %r1, %r1, 0x20
	cmpwi %r3, IRAI_THRESH_GROUP_9
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
	addi  %r3, %r3, 6130
	bl swGet
	cmpwi %r3, 0
bIraiRenderRedCheckReturn:
	b 0
