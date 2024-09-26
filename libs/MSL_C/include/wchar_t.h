#ifndef WCHAR_T_H
#define WCHAR_T_H

#if (!defined(__cplusplus))
    #ifdef __cplusplus
        extern "C" {
    #endif

    typedef unsigned short wchar_t;
    typedef wchar_t wint_t;	

    #ifdef __cplusplus
    }
    #endif
#endif

#endif // WCHAR_T_H