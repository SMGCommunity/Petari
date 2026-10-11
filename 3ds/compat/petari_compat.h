// Force-included (-include) into every Petari translation unit on non-CodeWarrior builds (3DS port).
#pragma once
#ifndef __MWERKS__
// CodeWarrior section/linkage attributes (e.g. __declspec(section ".init")) have no ARM meaning; drop them.
#define __declspec(x)
#ifndef INFINITY
#define INFINITY (__builtin_inff())
#endif
#ifndef NAN
#define NAN (__builtin_nanf(""))
#endif
#endif
