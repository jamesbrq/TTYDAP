.global asmFixEvtMapBlendSetFlagFollowerCrashStart
.global asmFixEvtMapBlendSetFlagFollowerCrashBranchBack
# asmFixEvtMapBlendSetFlagFollowerCrashStart and asmFixEvtMapBlendSetFlagFollowerCrashBranchBack need to be used in at least one subrel, so they cannot be set to hidden

asmFixEvtMapBlendSetFlagFollowerCrashStart:
# r3 already contains followerPtr
li %r4,0 # False
bl cFixEvtMapSetFlagCrash

# Restore the overwritten instruction
cmpwi %r31,1

asmFixEvtMapBlendSetFlagFollowerCrashBranchBack:
b 0
