.global asmFixBlooperCrash1
# asmFixBlooperCrash1 needs to be used in at least one subrel, so it cannot be set to hidden

asmFixBlooperCrash1:
mr %r3,%r31 # unkValue
# r4 already contains battleUnitPointer
b cFixBlooperCrash1
