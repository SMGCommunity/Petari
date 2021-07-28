.text

.include "macros.inc"

.global fmod
fmod:
/* 80526E20 00522360  4B FF C8 E0 */	b __ieee754_fmod
