.global win_log_mapGX_arr
.hidden win_log_mapGX_arr

.global bMapGXArrInject
.global bMapGXArrInjectReturn
.global bMapGXArrIncrement
.global bMapGXArrIncrementReturn
.global bMapGXArrFlagCheck
.global bMapGXArrFlagCheckReturn
.global bWinLogArrInject
.global bWinLogArrInjectReturn
.global bWinLogArrIncrement
.global bWinLogArrIncrementReturn
.global bWinLogArrFlagCheck
.global bWinLogArrFlagCheckReturn
.global bChapterClearCheck
.global bJohoyaSeqAddition
.global bPrintPartyErrorFix
.global bPrintPartyErrorFixReturn
.global bPrintPartyAddErrorFix
.global bPrintPartyAddErrorFixReturn
.global bKururingFloorCapture
.global bKururingFloorCaptureReturn
.global bKururingFloorRelease
.global bKururingFloorReleaseReturn
.global bPowerupblkCapture
.global bPowerupblkCaptureReturn
.global bPowerupblkRelease
.global bPowerupblkReleaseReturn
.global bPeachPointer
.global bPeachReturn
.global bShopFlagCheck
.global bShopFlagCheckReturn
.global bCoconutCheck
.global bCoconutCheckReturn
.global bGreenZoneMaxInit
.global bGreenZoneMaxInitReturn
.global bMonosiriItemCheck
.global bMonosiriItemCheckReturn
.global bMonosiriBucket
.global bMonosiriBucketReturn
.global bHpColorBack
.global bHpColorBackReturn
.global bHpColorFront
.global bHpColorFrontReturn
.global bMonosiriBucketExtra
.global bExpMultiplier
.global bExpMultiplierReturn
.global bBlockVisibility
.global bBlockVisibilityReturn
.global bShopDesc
.global bShopDescReturn

# All of the global symbols in this file excluding win_log_mapGX_arr need to be used in at least one subrel, so they cannot be set to hidden

# intended to be run once before the loop in main_mapGX
bMapGXArrInject:
	addi %r30, %r3, 0x7918 #Original Instruction
	lis %r3, win_log_mapGX_arr@ha
	addi %r28, %r3, win_log_mapGX_arr@l
bMapGXArrInjectReturn:
	b 0

# intended to be run at the end of every loop in main_mapGX
bMapGXArrIncrement:
	addi %r30, %r30, 0xC #Original Intruction
	addi %r28, %r28, 0x1
bMapGXArrIncrementReturn:
	b 0

# intended to be run when GSW(0) is checked
bMapGXArrFlagCheck:
	lbz %r3, 0x0(%r28)
	addi %r3, %r3, 0x189C
	bl swGet
bMapGXArrFlagCheckReturn:
	b 0

# intended to be run once before the loop in main_winLogMain
bWinLogArrInject:
	fadds %f27, %f0, %f7 #Original Instruction
	lis %r3, win_log_mapGX_arr@ha
	addi %r10, %r3, win_log_mapGX_arr@l
bWinLogArrInjectReturn:
	b 0

# intended to be run at the end of every loop in main_winLogMain
bWinLogArrIncrement:
	addi %r27, %r27, 0xC #Original Intruction
	addi %r10, %r10, 0x1
bWinLogArrIncrementReturn:
	b 0

# intended to be run when GSW(0) is checked
bWinLogArrFlagCheck:
	lbz %r3, 0x0(%r10)
	addi %r3, %r3, 0x189C
	bl swGet
bWinLogArrFlagCheckReturn:
	b 0

bChapterClearCheck:
	mflr %r28
	li %r3, 0x72
	li %r30, 0x1
	li %r29, 0x0
ChapterClearLoop:
	bl pouchCheckItem
	cmpwi %r3, 0x0
	beq ChapterClearIncrement
	addi %r29, %r29, 0x1
ChapterClearIncrement:
	addi %r3, %r30, 0x72
	addi %r30, %r30, 0x1
	cmpwi %r3, 0x78
	ble ChapterClearLoop
	mr %r3, %r29
	li %r30, 0x0
	li %r29, 0x0
	mtlr %r28
	li %r28, 0x0
	blr

bJohoyaSeqAddition:
	mflr %r28
	li %r3, 0x6A4
	li %r4, 0x1
	li %r5, 0x0
JohoyaSeqLoop:
	bl swByteGet
	add %r5, %r5, %r3
	addi %r3, %r4, 0x6A4
	addi %r4, %r4, 0x1
	cmpwi %r3, 0x6B8
	ble JohoyaSeqLoop
	mr %r3, %r5
	mtlr %r28
	li %r28, 0x0
	blr

bPrintPartyAddErrorFix:
	bl strcat
	bl msgSearch
	lis %r4, 0x802C
	addi %r4, %r4, 0x30DC
	cmpw %r3, %r4
	bne addErrorEnd
	mr %r4, %r30
	lis %r3, 0x803D
	ori %r3, %r3, 0xF508
	bl strcpy
	lis %r3, 0x803D
	ori %r3, %r3, 0xF508
	lis %r4, 0x8042
	addi %r4, %r4, 0x1714
	bl strcat
addErrorEnd:
bPrintPartyAddErrorFixReturn:
	b 0

bPrintPartyErrorFix:
	bl strcat
	bl msgSearch
	lis %r4, 0x802C
	addi %r4, %r4, 0x30DC
	cmpw %r3, %r4
	bne errorEnd
	mr %r4, %r30
	lis %r3, 0x803D
	ori %r3, %r3, 0xF548
	bl strcpy
	lis %r3, 0x803D
	ori %r3, %r3, 0xF548
	lis %r4, 0x8042
	addi %r4, %r4, 0x1714
	bl strcat
errorEnd:
bPrintPartyErrorFixReturn:
	b 0

bKururingFloorCapture:
	mr %r18, %r3
	mr %r3, %r26 #Original Intruction
bKururingFloorCaptureReturn:
	b 0
	
bKururingFloorRelease:
	stw %r18, 0x1C0(%r3)
	li %r0, 0x0 #Original Intruction
bKururingFloorReleaseReturn:
	b 0

bPowerupblkCapture:
	cmpwi %r4, 0x0
	bne bPowerupblkNorm
	li %r4, 0x57
bPowerupblkNorm:
	mr %r19, %r4
	li %r4, 0x0
	mr %r3, %r30 #Original Intruction
bPowerupblkCaptureReturn:
	b 0
	
bPowerupblkRelease:
	stw %r19, 0x1D8(%r3)
	mr %r28, %r3 #Original Intruction
bPowerupblkReleaseReturn:
	b 0

bPeachPointer:
	lis %r3, _next_map@h
	ori %r4, %r3, _next_map@l
	lis %r3, str_pik_04_802c075c@ha
	addi %r3, %r3, str_pik_04_802c075c@l
	bl strcmp
	cmpwi %r3, 0x0
	beq pikBreakfast
	lis %r3, _next_map@h
	ori %r4, %r3, _next_map@l
	lis %r3, str_bom_02_802c0378@ha
	addi %r3, %r3, str_bom_02_802c0378@l
	bl strcmp
	cmpwi %r3, 0x0
	beq bomBreakfast
	lis %r6, 0xF840 #usuBreakfast
	ori %r6, %r6, 0x735A
	b breakfastValueRestore	
bomBreakfast:
	lis %r6, 0xF840
	ori %r6, %r6, 0x735F
	b breakfastValueRestore	
pikBreakfast:
	lis %r6, 0xF840
	ori %r6, %r6, 0x735E
breakfastValueRestore:
	lis %r3, main_wp_breakfast@h
	ori %r3, %r3, main_wp_breakfast@l
	lwz %r3, 0x0(%r3)
	lwz %r3, 0x0(%r3)
	lwz %r4, 0x0(%r3)
	lis %r3, str_breakfast_802ed418@h
	ori %r3, %r3, str_breakfast_802ed418@l
	li %r5, 0x10
bPeachReturn:
	b 0

bShopFlagCheck:
	mr %r3, %r0
	mr %r4, %r27
	bl checkShopFlag
bShopFlagCheckReturn:
	b 0

bGreenZoneMaxInit:
	lwz %r4, 0x1cc8(%r29)
	cmpwi %r4, 0x1
	bne bGreenZoneMaxInitNorm
	lis %r4, 0x42CF
	ori %r4, %r4, 0x0000
	stw %r4, 0x18(%r31)
	b bGreenZoneMaxInitEnd
bGreenZoneMaxInitNorm:
	stfs %f0, 0x18(%r31)
bGreenZoneMaxInitEnd:
	li %r4, 0x0
bGreenZoneMaxInitReturn:
	b 0

bMonosiriItemCheck:
	stwu %r1, -0x80(%r1)
	mflr %r0
	stw %r0, 0x84(%r1)
	stmw %r3, 0x8(%r1)
	bl monosiriItemCheck
	lmw %r3, 0x8(%r1)
	lwz %r0, 0x84(%r1)
	mtlr %r0
	addi %r1, %r1, 0x80
	mr %r27, %r3
bMonosiriItemCheckReturn:
	b 0

bMonosiriBucket:
	lis %r3, monosiriSWArr@ha
	addi %r3, %r3, monosiriSWArr@l
	mulli %r0, %r28, 0xC
	add %r26, %r3, %r0
	lwz %r3, 0x0(%r26)
	cmpwi %r3, 0x1000
	blt bMonosiriBucketNorm
	bl swGet
	b bMonosiriBucketEnd
bMonosiriBucketNorm:
	bl swByteGet
bMonosiriBucketEnd:
	mr %r30, %r3
	li %r3, 0x1
	bl partyChkJoin
	cmpwi %r3, 0x1
	beq bGoombellaSkip
	li %r30, 0x0
bGoombellaSkip:
	lwz %r0, 0x8(%r26)
bMonosiriBucketReturn:
	b 0

bMonosiriBucketExtra:
	stwu %r1, -0x16(%r1)
	mflr %r4 
	stw %r4, 0x20(%r1)
	bl swGet
	cmpwi %r3, 0x0
	bne bMonosiriBucketExtraSkip
	li %r3, 0x1
	bl partyChkJoin
	cmpwi %r3, 0x1
	bne bMonosiriBucketExtraStop
	li %r3, 0x0
	b bMonosiriBucketExtraSkip
bMonosiriBucketExtraStop:
	li %r3, 0x1
bMonosiriBucketExtraSkip:
	lwz %r4, 0x20(%r1)
	mtlr %r4
	addi %r1, %r1, 0x16
	blr

bHpColorBack:
	stwu %r1, -0x80(%r1)
	mflr %r3
	stmw %r3, 0x8(%r1)
	lwz %r3, 0x4(%r29)
	addi %r3, %r3, 0x117A
	bl swGet
	cmpwi %r3, 0x1
	beq bHpColorBackGreen
	lwz %r0, 0x0(%r30)
	b bHpColorBackEnd
bHpColorBackGreen:
	lis %r0, 0x22FF
	ori %r0, %r0, 0xFFFF
bHpColorBackEnd:
	lmw %r3, 0x8(%r1)
	mtlr %r3
	li %r3, 0x11
	addi %r1, %r1, 0x80
bHpColorBackReturn:
	b 0

bHpColorFront:
	stwu %r1, -0x80(%r1)
	mflr %r3
	stmw %r3, 0x8(%r1)
	lwz %r3, 0x4(%r29)
	addi %r3, %r3, 0x117A
	bl swGet
	cmpwi %r3, 0x1
	beq bHpColorFrontGreen
	lwz %r0, 0x0(%r30)
	b bHpColorFrontEnd
bHpColorFrontGreen:
	lis %r0, 0x22FF
	ori %r0, %r0, 0xFFFF
bHpColorFrontEnd:
	lmw %r3, 0x8(%r1)
	mtlr %r3
	li %r3, 0x12
	addi %r1, %r1, 0x80
bHpColorFrontReturn:
	b 0

bExpMultiplier:
	bl applyExpMultiplier
	sth %r3, 0x8(%r28)
bExpMultiplierReturn:
	b 0

bBlockVisibility:
    mr %r30, %r3
    stwu %r1, -104(%r1)
    stmw %r4, 12(%r1)
    mr %r3, %r27
    bl getBlockVisibility
    lmw %r4, 12(%r1)
    mr %r27, %r3
    addi %r1, %r1, 104
bBlockVisibilityReturn:
    b 0

bShopDesc:
	bl shopItemDescription
	bl msgSearch
bShopDescReturn:
 	b 0

win_log_mapGX_arr:
	.byte 0x0
	.byte 0x1
	.byte 0x1
	.byte 0x1
	.byte 0x2
	.byte 0x3
	.byte 0x3
	.byte 0x3
	.byte 0x3
	.byte 0x4
	.byte 0x4
	.byte 0x4
	.byte 0x4
	.byte 0x5
	.byte 0x6
	.byte 0x6
	.byte 0x7
	.byte 0x7
	.byte 0x7
	.byte 0x8
	.byte 0x8
	.byte 0x8
	.byte 0x8
	.byte 0x8
	.byte 0x8
	.byte 0x8
	.byte 0x8
	.byte 0x8
	.byte 0x8
	.byte 0x8
	.byte 0x8
	.byte 0x8
	.byte 0x9
	.byte 0xA
	.byte 0xA
	.byte 0xA
	.byte 0xA
	.byte 0xA
	.byte 0xA
	.byte 0xA
	.byte 0xA
	.byte 0xA
	.byte 0xB
	.byte 0xB
	.byte 0xB
	.byte 0xB
	.byte 0xB
	.byte 0xB
	.byte 0xC
	.byte 0xC
	.byte 0xC
	.byte 0xC
	.byte 0xC
	.byte 0xC
	.byte 0xC
	.byte 0xC
	.byte 0xC
	.byte 0xC
	.byte 0xD
	.byte 0xD
	.byte 0xD
	.byte 0x15 # Train
	.byte 0x15
	.byte 0x15
	.byte 0x15
	.byte 0xE
	.byte 0xE
	.byte 0xE
	.byte 0xE
	.byte 0xF
	.byte 0xF
	.byte 0xF
	.byte 0xF
	.byte 0xF
	.byte 0x10
	.byte 0x10
	.byte 0x10
	.byte 0x11
	.byte 0x12
	.byte 0x12
	.byte 0x12
	.byte 0x12
	.byte 0x12
	.byte 0x12
	.byte 0x12
	.byte 0x12
	.byte 0x13
	.byte 0x13
	.byte 0x13
	.byte 0x13
	.byte 0x14
	.byte 0x16 # Riddle Tower
	.byte 0x14

monosiriSWArr:
	.4byte 0x000017A8
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0x000006A5
	.4byte 0x00000012
	.4byte 0x00000006
	.4byte 0x000006A5
	.4byte 0x00000014
	.4byte 0x00000009
	.4byte 0x00001110
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0x000006A5
	.4byte 0x00000012
	.4byte 0x00000006
	.4byte 0x000006A5
	.4byte 0x00000013
	.4byte 0x00000007
	.4byte 0x000006AF
	.4byte 0x00000016
	.4byte 0x00000005
	.4byte 0x00001110
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0x000006AF
	.4byte 0x00000017
	.4byte 0x00000008
	.4byte 0x000006B1
	.4byte 0x0000001D
	.4byte 0x0000000B
	.4byte 0x000006B1
	.4byte 0x0000001E
	.4byte 0x0000000B
	.4byte 0x000006B0
	.4byte 0x00000021
	.4byte 0x00000002
	.4byte 0x000006B1
	.4byte 0x00000022
	.4byte 0x0000000B
	.4byte 0x000006A7
	.4byte 0x00000025
	.4byte 0x00000003
	.4byte 0x000006A7
	.4byte 0x00000026
	.4byte 0x00000003
	.4byte 0x000006A7
	.4byte 0x0000002C
	.4byte 0x00000003
	.4byte 0x000006A7
	.4byte 0x0000002D
	.4byte 0x00000003
	.4byte 0x000006A7
	.4byte 0x00000040
	.4byte 0x0000000E
	.4byte 0x000006A7
	.4byte 0x00000041
	.4byte 0x00000014
	.4byte 0x000006B5
	.4byte 0x0000005D
	.4byte 0x0000000A
	.4byte 0x000006B5
	.4byte 0x00000063
	.4byte 0x00000011
	.4byte 0x000006AA
	.4byte 0x0000006B
	.4byte 0x00000024
	.4byte 0x000006AB
	.4byte 0x00000075
	.4byte 0x00000010
	.4byte 0x000006AB
	.4byte 0x00000076
	.4byte 0x00000010
	.4byte 0x000006AB
	.4byte 0x0000001C
	.4byte 0x00000010
	.4byte 0x000006A6
	.4byte 0x0000001C
	.4byte 0x00000001
	.4byte 0x000006AB
	.4byte 0x00000078
	.4byte 0x00000010
	.4byte 0x000006AB
	.4byte 0x00000077
	.4byte 0x00000010
	.4byte 0x000006AB
	.4byte 0x00000079
	.4byte 0x00000010
	.4byte 0x000006AC
	.4byte 0x00000083
	.4byte 0x00000012
	.4byte 0x000006AC
	.4byte 0x00000084
	.4byte 0x00000010
	.4byte 0x000006B0
	.4byte 0x00000085
	.4byte 0x00000002
	.4byte 0x000006AC
	.4byte 0x00000085
	.4byte 0x00000009
	.4byte 0x000006B0
	.4byte 0x00000086
	.4byte 0x00000002
	.4byte 0x000006AC
	.4byte 0x00000086
	.4byte 0x00000009
	.4byte 0x000006B3
	.4byte 0x0000004D
	.4byte 0x00000008
	.4byte 0x000006AC
	.4byte 0x00000087
	.4byte 0x00000009
	.4byte 0x000006AC
	.4byte 0x00000090
	.4byte 0x00000010
	.4byte 0x000006AC
	.4byte 0x00000091
	.4byte 0x00000010
	.4byte 0x000006AC
	.4byte 0x00000092
	.4byte 0x00000010
	.4byte 0x000006AC
	.4byte 0x00000093
	.4byte 0x00000010
	.4byte 0x000006AC
	.4byte 0x00000096
	.4byte 0x00000012