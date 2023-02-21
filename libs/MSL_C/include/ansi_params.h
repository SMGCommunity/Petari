#ifndef ANSI_PARAMS_H
#define ANSI_PARAMS_H

#define _MSL_CANT_THROW __attribute__((nothrow))

#define _MSL_THROW throw()

#ifndef _MSL_LOCALDATA
#define _MSL_LOCALDATA(_a) _a
#endif

#define __std(ref) ref

#endif // ANSI_PARAMS_H