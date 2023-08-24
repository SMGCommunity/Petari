#include "JSystem/JGadget/hashcode.h"
/*
//#pragma optimization_level 0

namespace JGadget {
    s32 doCrazyThings(s32 &term, s32 seed) {return term = seed;}
    inline bool gg(char &dst, const char *data, s32 terminator) {
        dst = *data;
        return *data != terminator;
    }
    inline void helper(s32 &acc, const char *data) {
        //doCrazyThings(acc, acc);
        //s32 tmp1 = 0;
        // = 0;
        //char tmpValue2;
        s32 term = 0;
        s32 &terminator = term;
        //char &uni = unimportant;
        char unimportant;
        while(gg(unimportant, data, term)) {
            //terminator = *data - *data;
            //s32 tmp2 = tmp1;
            //tmp1 = acc + 1;
            //tmp1 = tmp2;
          //  doCrazyThings(tmp1, tmp1);
            //unimportant = *data;
            s32 tmp = acc * 31;
            acc = unimportant;
            doCrazyThings(terminator, terminator);
            data++;
            acc += tmp;
            //acc += tmp;
        }
    }
    void mull(s32 &a, s32 b) {a = b * 31;}
    inline void function(const char *data, s32 &r4, s32 &acc, char &r6) {
        for(s32 terminator = *data - *data; (r4 = r6 = *data) != terminator;) {
            mull(r4, acc);
            acc = r6;
            acc += r4;
            data++;
        }
    }
    char doubleBS(char &a, char b) {return a = b;}
    u32 getHashCode(const char *data) {
        s32 acc = 0;
        //doCrazyThings(acc, acc);
        //s32 tmp1 = 0;
        // = 0;
        //char tmpValue2;
        helper(acc, data);
        return acc;

        /*s32 r4 = 0, acc = 0;
        char r6 = 0;
        function(data, r4, acc, r6);
        return acc;*/
        
        //s32 tmp = *data - *data;
        //return helper(data, tmp);
        /*char acc = 0;
        char tmp = *data;// + data[1];
        tmp *= data[1];
        acc = tmp;
        //doCrazyThings(tmp, tmp);
        acc += tmp;
        return acc;*/
/*    }
    
}*/
