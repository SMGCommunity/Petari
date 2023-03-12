#pragma once

#ifdef __cplusplus
extern "C" {
#endif

__declspec(section ".init") extern void __init_hardware(void);
extern void __init_user(void);
__declspec(section ".init") extern void __flush_cache(void *, unsigned int);

#ifdef __cplusplus
}
#endif
