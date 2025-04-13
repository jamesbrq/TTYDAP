.global win_log_mapGX_arr
.hidden win_log_mapGX_arr

.global bMapGXArrInject
.global bMapGXArrInjectReturn
.global bMapGXArrIncrement
.global bMapGXArrIncrementReturn
.global bMapGXChSplit
.global bMapGXChSplitReturn
.global bWinLogArrInject
.global bWinLogArrInjectReturn
.global bWinLogArrIncrement
.global bWinLogArrIncrementReturn
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

# All of the global symbols in this file excluding win_log_mapGX_arr need to be used in at least one subrel, so they cannot be set to hidden

bMapGXArrInject:
	lis %r3, win_log_mapGX_arr@ha
	addi %r28, %r3, win_log_mapGX_arr@l
	lhz %r3, 0x0(%r28)
bMapGXArrInjectReturn:
	b 0

bMapGXArrIncrement:
	addi %r30, %r30, 0xC #Original Intruction
	addi %r28, %r28, 0x2
bMapGXArrIncrementReturn:
	b 0

bMapGXChSplit:
	li %r3, 0x06AF
	bl swByteGet
	cmpwi %r3, 0x3
bMapGXChSplitReturn:
	b 0

bWinLogArrInject:
	lis %r3, win_log_mapGX_arr@ha
	addi %r10, %r3, win_log_mapGX_arr@l
	lhz %r3, 0x0(%r10)
bWinLogArrInjectReturn:
	b 0

bWinLogArrIncrement:
	addi %r27, %r27, 0xC #Original Intruction
	addi %r10, %r10, 0x2
bWinLogArrIncrementReturn:
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
	lis %r3, wp_breakfast@h
	ori %r3, %r3, wp_breakfast@l
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

win_log_mapGX_arr:
	.2byte 0x06A4
	.2byte 0x06A4
	.2byte 0x06A4
	.2byte 0x06A4
	.2byte 0x06A5
	.2byte 0x06A5
	.2byte 0x06A5
	.2byte 0x06A5
	.2byte 0x06A5
	.2byte 0x06AF
	.2byte 0x06AF
	.2byte 0x06AF
	.2byte 0x06AF
	.2byte 0x06A6
	.2byte 0x06A6
	.2byte 0x06A6
	.2byte 0x06A6
	.2byte 0x06A6
	.2byte 0x06A6
	.2byte 0x06A7
	.2byte 0x06A7
	.2byte 0x06A7
	.2byte 0x06A7
	.2byte 0x06A7
	.2byte 0x06A7
	.2byte 0x06A7
	.2byte 0x06A7
	.2byte 0x06A7
	.2byte 0x06A7
	.2byte 0x06A7
	.2byte 0x06A7
	.2byte 0x06A7
	.2byte 0x06A8
	.2byte 0x06A8
	.2byte 0x06A8
	.2byte 0x06A8
	.2byte 0x06A8
	.2byte 0x06A8
	.2byte 0x06A8
	.2byte 0x06A8
	.2byte 0x06A8
	.2byte 0x06A8
	.2byte 0x06B2
	.2byte 0x06B2
	.2byte 0x06B2
	.2byte 0x06B2
	.2byte 0x06B2
	.2byte 0x06B2
	.2byte 0x06A9
	.2byte 0x06A9
	.2byte 0x06A9
	.2byte 0x06A9
	.2byte 0x06A9
	.2byte 0x06A9
	.2byte 0x06A9
	.2byte 0x06A9
	.2byte 0x06A9
	.2byte 0x06A9
	.2byte 0x06B5
	.2byte 0x06B5
	.2byte 0x06B5
	.2byte 0x06B8
	.2byte 0x06B8
	.2byte 0x06B8
	.2byte 0x06B8
	.2byte 0x06B8
	.2byte 0x06B8
	.2byte 0x06B8
	.2byte 0x06B8
	.2byte 0x06AA
	.2byte 0x06AA
	.2byte 0x06AA
	.2byte 0x06AA
	.2byte 0x06AA
	.2byte 0x06AA
	.2byte 0x06AA
	.2byte 0x06AA
	.2byte 0x06AB
	.2byte 0x06AB
	.2byte 0x06AB
	.2byte 0x06AB
	.2byte 0x06AB
	.2byte 0x06AB
	.2byte 0x06AB
	.2byte 0x06AB
	.2byte 0x06AB
	.2byte 0x06AB
	.2byte 0x06AB
	.2byte 0x06AB
	.2byte 0x06AB
	.2byte 0x06AC
	.2byte 0x06AC
	.2byte 0x06AC