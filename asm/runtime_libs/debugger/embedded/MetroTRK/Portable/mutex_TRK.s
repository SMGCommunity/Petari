.text

.include "macros.inc"

.global TRKReleaseMutex
TRKReleaseMutex:
/* 80529898 00524DD8  38 60 00 00 */	li r3, 0
/* 8052989C 00524DDC  4E 80 00 20 */	blr 

.global TRKAcquireMutex
TRKAcquireMutex:
/* 805298A0 00524DE0  38 60 00 00 */	li r3, 0
/* 805298A4 00524DE4  4E 80 00 20 */	blr 

.global TRKInitializeMutex
TRKInitializeMutex:
/* 805298A8 00524DE8  38 60 00 00 */	li r3, 0
/* 805298AC 00524DEC  4E 80 00 20 */	blr 
