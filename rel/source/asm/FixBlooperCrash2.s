.global asmFixBlooperCrash2
# asmFixBlooperCrash2 needs to be used in at least one subrel, so it cannot be set to hidden

asmFixBlooperCrash2:
# Check if BattleGetUnitPtr returned a proper pointer
cmpwi %r3,0
beq- exitFunction

# Restore the overwritten instruction
rlwinm %r0,%r31,2,0,29
blr

exitFunction:
b (btlevtcmd_GetUnitWork + 0x6C) # 0x6C offset is the same for all retail versions
