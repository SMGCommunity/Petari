#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <functional.h>

template<class InputIterator, class Function>
inline Function for_each(InputIterator first, InputIterator last, Function f) {
    for (; first != last; ++first) {
        f(*first);
    }

    return f;
}

#endif // ALGORITHM_H