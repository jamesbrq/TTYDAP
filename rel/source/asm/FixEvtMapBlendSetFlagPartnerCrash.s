.global asmFixEvtMapBlendSetFlagPartnerCrashStart
.global asmFixEvtMapBlendSetFlagPartnerCrashBranchBack
# asmFixEvtMapBlendSetFlagPartnerCrashStart and asmFixEvtMapBlendSetFlagPartnerCrashBranchBack need to be used in at least one subrel, so they cannot be set to hidden

asmFixEvtMapBlendSetFlagPartnerCrashStart:
# r3 already contains partnerPtr
li %r4,1 # True
bl cFixEvtMapSetFlagCrash

# Restore the overwritten instruction
cmpwi %r31,1

asmFixEvtMapBlendSetFlagPartnerCrashBranchBack:
b 0
