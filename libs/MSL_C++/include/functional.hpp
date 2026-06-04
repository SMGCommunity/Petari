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

    template < class Predicate >
    struct unary_negate : public unary_function< typename Predicate::argument_type, bool > {
        explicit unary_negate(const Predicate& pred) : mPred(pred) {
        }
        bool operator()(const typename Predicate::argument_type& x) const {
            return (bool)!mPred(x);
        }
        Predicate mPred;
    };

    template < class Predicate >
    unary_negate< Predicate > not1(const Predicate& pred) {
        return unary_negate< Predicate >(pred);
    }

    template < class Arg, class Result >
    class pointer_to_unary_function : public unary_function< Arg, Result > {
    public:
        explicit pointer_to_unary_function(Result (*f)(Arg)) : mF(f) {
        }
        Result operator()(Arg x) const {
            return mF(x);
        }
        Result (*mF)(Arg);
    };

    template < class Arg, class Result >
    pointer_to_unary_function< Arg, Result > ptr_fun(Result (*f)(Arg)) {
        return pointer_to_unary_function< Arg, Result >(f);
    }

}  // namespace std

#endif  // FUNCTIONAL_H
