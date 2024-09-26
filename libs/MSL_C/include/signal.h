#ifndef SIGNAL_H
#define SIGNAL_H

typedef void (*__signal_func_ptr)(int);

int raise(int);

#endif // SIGNAL_H