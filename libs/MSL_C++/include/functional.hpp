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

    // Binders

    template < class Func, class Type >
    class binder1st : public unary_function< typename Func::second_argument_type, typename Func::result_type > {
    public:
        typedef unary_function< typename Func::second_argument_type, typename Func::result_type > Base;
        typedef typename Base::argument_type Arg;
        typedef typename Base::result_type Result;

        binder1st(const Func& mf, const Type& v) : mf_(mf), v_(v){};

        Result operator()(const Arg& a) const {
            return mf_(a, v_);
        }

        Result operator()(Arg& a) const {
            return mf_(a, v_);
        }

    private:
        Func mf_;
        Type v_;
    };

    template < class Func, class Type >
    class binder2nd : public unary_function< typename Func::first_argument_type, typename Func::result_type > {
    public:
        typedef unary_function< typename Func::first_argument_type, typename Func::result_type > Base;
        typedef typename Base::argument_type Arg;
        typedef typename Base::result_type Result;

        binder2nd(const Func& mf, const Type& v) : mf_(mf), v_(v){};

        Result operator()(const Arg& a) const {
            return mf_(a, v_);
        }

        Result operator()(Arg& a) const {
            return mf_(a, v_);
        }

    private:
        Func mf_;
        Type v_;
    };

    template < class Func, class Type >
    binder1st< Func, const Type& > bind1st(const Func& mf, const Type& a) {
        return binder1st< Func, const Type& >(mf, a);
    }

    template < class Func, class Type >
    binder2nd< Func, const Type& > bind2nd(const Func& mf, const Type& a) {
        return binder2nd< Func, const Type& >(mf, a);
    }

    // Mem funcs

    template < class Return, class Type >
    class mem_fun_t : public unary_function< Type*, Return > {
    public:
        explicit mem_fun_t(Return (Type::*mf)()) : mf_(mf){};

        Return operator()(Type* t) const {
            return (t->*mf_)();
        }

    private:
        Return (Type::*mf_)();
    };

    template < class Return, class Type >
    class const_mem_fun_t : public unary_function< const Type*, Return > {
    public:
        explicit const_mem_fun_t(Return (Type::*mf)() const) : mf_(mf){};

        Return operator()(const Type* t) const {
            return (t->*mf_)();
        }

    private:
        Return (Type::*mf_)() const;
    };

    template < class Result, class Type, class Arg >
    class mem_fun1_t : public binary_function< Type*, Arg, Result > {
    public:
        explicit mem_fun1_t(Result (Type::*mf)(Arg)) : mf_(mf){};

        Result operator()(Type* t, Arg a) const {
            return (t->*mf_)(a);
        }

    private:
        Result (Type::*mf_)(Arg);
    };

    template < class Result, class Type, class Arg >
    class const_mem_fun1_t : public binary_function< const Type*, Arg, Result > {
    public:
        explicit const_mem_fun1_t(Result (Type::*mf)(Arg) const) : mf_(mf){};

        Result operator()(const Type* t, Arg a) const {
            return (t->*mf_)(a);
        }

    private:
        Result (Type::*mf_)(Arg) const;
    };

    template < class Result, class Type >
    inline mem_fun_t< Result, Type > mem_func(Result (Type::*f)()) {
        return mem_fun_t< Result, Type >(f);
    }

    template < class Result, class Type >
    inline const_mem_fun_t< Result, Type > mem_func(Result (Type::*f)() const) {
        return const_mem_fun_t< Result, Type >(f);
    }

    template < class Result, class Type, class Arg >
    inline mem_fun1_t< Result, Type, Arg > mem_func(Result (Type::*f)(Arg)) {
        return mem_fun1_t< Result, Type, Arg >(f);
    }

    template < class Result, class Type, class Arg >
    inline const_mem_fun1_t< Result, Type, Arg > mem_func(Result (Type::*f)(Arg) const) {
        return const_mem_fun1_t< Result, Type, Arg >(f);
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

    template < class Arg1, class Arg2, class Result >
    class pointer_to_binary_function : public binary_function< Arg1, Arg2, Result > {
    public:
        explicit pointer_to_binary_function(Result (*f)(Arg1, Arg2)) : mF(f) {
        }
        Result operator()(Arg1 x, Arg2 y) const {
            return mF(x, y);
        }
        Result (*mF)(Arg1, Arg2);
    };

    template < class Arg1, class Arg2, class Result >
    pointer_to_binary_function< Arg1, Arg2, Result > ptr_fun(Result (*f)(Arg1, Arg2)) {
        return pointer_to_binary_function< Arg1, Arg2, Result >(f);
    }

}  // namespace std

#endif  // FUNCTIONAL_H
