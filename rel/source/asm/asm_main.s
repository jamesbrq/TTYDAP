.global win_log_mapGX_arr
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
.global bChapterClearCheckReturn
.global bJohoyaSeqAddition

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
	cmpwi %r3, 0x1
	bne ChapterClearIncrement
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
	blr

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