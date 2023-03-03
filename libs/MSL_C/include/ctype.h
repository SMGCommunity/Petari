#ifndef CTYPE_H
#define CTYPE_H

extern const unsigned char __lower_map[0x100];
extern const unsigned short __ctype_mapC[0x100];

int tolower(int);

#endif // CTYPE_H