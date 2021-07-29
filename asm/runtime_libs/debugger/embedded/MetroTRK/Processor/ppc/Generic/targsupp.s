.text

.include "macros.inc"

.global TRKAccessFile
TRKAccessFile:
/* 8052B470 005269B0  0F E0 00 00 */	twui r0, 0
/* 8052B474 005269B4  4E 80 00 20 */	blr

.global TRKOpenFile
TRKOpenFile:
/* 8052B478 005269B8  0F E0 00 00 */	twui r0, 0
/* 8052B47C 005269BC  4E 80 00 20 */	blr

.global TRKCloseFile
TRKCloseFile:
/* 8052B480 005269C0  0F E0 00 00 */	twui r0, 0
/* 8052B484 005269C4  4E 80 00 20 */	blr 

.global TRKPositionFile
TRKPositionFile:
/* 8052B488 005269C8  0F E0 00 00 */	twui r0, 0
/* 8052B48C 005269CC  4E 80 00 20 */	blr 
