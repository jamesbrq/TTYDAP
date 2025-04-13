#ifdef TTYD_JP

.global asmCrashScreenDecrementYPosStart
.global asmCrashScreenDecrementYPosBranchBack
# asmCrashScreenDecrementYPosStart and asmCrashScreenDecrementYPosBranchBack need to be used in at least one subrel, so they cannot be set to hidden

asmCrashScreenDecrementYPosStart:
bl cCrashScreenDecrementYPos
mr %r25,%r3 # posY

asmCrashScreenDecrementYPosBranchBack:
b 0

#endif
