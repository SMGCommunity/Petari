#ifndef FUNCTIONAL_H
#define FUNCTIONAL_H

namespace std {
    template < class Arg, class Result >
    struct unary_function {
        typedef Arg argument_type;
        typedef Result result_type;
    };

    template < class Arg1, class Arg2, class Result >
    struct binary_function {
        typedef Arg1 first_argument_type;
        typedef Arg2 second_argument_type;
        typedef Result result_type;
    };

    template < class S, class T >
    class mem_fun_t : public unary_function< T*, S > {
    public:
        explicit mem_fun_t(S (T::*mf)());

        S operator()(T* p) const;

    private:
        S (T::*mf_)();
    };

    template < class S, class T >
    inline mem_fun_t< S, T >::mem_fun_t(S (T::*mf)()) : mf_(mf) {
    }

    template < class S, class T >
    inline S mem_fun_t< S, T >::operator()(T* p) const {
        return (p->*mf_)();
    }

    template < class S, class T >
    inline mem_fun_t< S, T > mem_func(S (T::*f)()) {
        return mem_fun_t< S, T >(f);
    }

    template < class S, class T >
    class const_mem_fun_t : public unary_function< T*, S > {
    public:
        explicit const_mem_fun_t(S (T::*mf)() const);

        S operator()(T* p) const;

    private:
        S (T::*mf_)() const;
    };

    template < class S, class T >
    inline const_mem_fun_t< S, T >::const_mem_fun_t(S (T::*mf)() const) : mf_(mf) {
    }

    template < class S, class T >
    inline S const_mem_fun_t< S, T >::operator()(T* p) const {
        return (p->*mf_)();
    }

    template < class S, class T >
    inline const_mem_fun_t< S, T > const_mem_func(S (T::*f)() const) {
        return const_mem_fun_t< S, T >(f);
    }
}  // namespace std

#endif  // FUNCTIONAL_H
