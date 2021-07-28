.text

.include "macros.inc"

.global udp_cc_post_stop
udp_cc_post_stop:
/* 8052C1E8 00527728  38 60 FF FF */	li r3, -1
/* 8052C1EC 0052772C  4E 80 00 20 */	blr

.global udp_cc_pre_continue
udp_cc_pre_continue:
/* 8052C1F0 00527730  38 60 FF FF */	li r3, -1
/* 8052C1F4 00527734  4E 80 00 20 */	blr

.global udp_cc_peek
udp_cc_peek:
/* 8052C1F8 00527738  38 60 00 00 */	li r3, 0
/* 8052C1FC 0052773C  4E 80 00 20 */	blr 

.global udp_cc_write
udp_cc_write:
/* 8052C200 00527740  38 60 00 00 */	li r3, 0
/* 8052C204 00527744  4E 80 00 20 */	blr 

.global udp_cc_read
udp_cc_read:
/* 8052C208 00527748  38 60 00 00 */	li r3, 0
/* 8052C20C 0052774C  4E 80 00 20 */	blr 

.global udp_cc_close
udp_cc_close:
/* 8052C210 00527750  38 60 FF FF */	li r3, -1
/* 8052C214 00527754  4E 80 00 20 */	blr 

.global udp_cc_open
udp_cc_open:
/* 8052C218 00527758  38 60 FF FF */	li r3, -1
/* 8052C21C 0052775C  4E 80 00 20 */	blr 

.global udp_cc_shutdown
udp_cc_shutdown:
/* 8052C220 00527760  38 60 FF FF */	li r3, -1
/* 8052C224 00527764  4E 80 00 20 */	blr 

.global udp_cc_initialize
udp_cc_initialize:
/* 8052C228 00527768  38 60 FF FF */	li r3, -1
/* 8052C22C 0052776C  4E 80 00 20 */	blr 
