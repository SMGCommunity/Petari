#ifndef SCPRIVATE_H
#define SCPRIVATE_H

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef u8 SCType;

#define SC_TYPE_MASK                ((SCType)0xe0)
#define SC_NAME_LENGTH_MASK         ((SCType)~SC_TYPE_MASK)

#ifdef __cplusplus
}
#endif

#endif // SCPRIVATE_H