#ifndef NUBEVENT_H
#define NUBEVENT_H

#include "portable/msgbuf.h"

typedef unsigned long NubEventID;

typedef enum NubEventType {
	kNullEvent,
	kShutdownEvent,
	kRequestEvent,
	kBreakpointEvent,
	kExceptionEvent,
	kSupportEvent
} NubEventType;

typedef struct NubEvent {
	NubEventType		fType;
	NubEventID			fID;
	MessageBufferID		fMessageBufferID;
} NubEvent;

void TRKConstructEvent(NubEvent *, NubEventType);
int TRKGetNextEvent(NubEvent *);
int TRKPostEvent(const NubEvent *);

#endif // NUBEVENT_H