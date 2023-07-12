#ifndef OSPLAYTIME_H
#define OSPLAYTIME_H

typedef enum {
    OSPLAYTIME_PERMANENT        = 0,
    OSPLAYTIME_LIMITED          = 1,
    OSPLAYTIME_LIMITED_LAUNCH   = 4,
    OSPLAYTIME_OTHER            = 9

} __OSPlayTimeType;

typedef void (*OSPlayTimeCallbackFunc)(void);

void __OSInitPlayTime(void);

#endif // OSPLAYTIME_H