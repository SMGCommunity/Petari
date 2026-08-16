#ifndef CTYPE_H
#define CTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

extern const unsigned char __lower_mapC[0x100];
extern const unsigned char __upper_mapC[0x100];
extern const unsigned short __ctype_mapC[0x100];

int tolower(int);

#ifdef __cplusplus
}
#endif

#endif  // CTYPE_H
