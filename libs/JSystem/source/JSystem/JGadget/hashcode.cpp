#include "JSystem/JGadget/hashcode.h"

//#pragma optimization_level 0

namespace JGadget {
    s32 doCrazyThings(s32 &term, s32 seed) {return term = seed;}
    // We need to create a percieved side-effect so that the compiler
    // falsely believes the initial value of dst is significant for the loop
    struct BS {
        s32 &acc;
        u8 &tt;
    };
    inline bool gg(u8 &dst, const char *data, s32 terminator) {
        //*((char *)data - 1) = dst;
        //const_cast<stupidididid &>(out2) = reinterpret_cast<volatile stupidididid &>(dst);
        dst = *data;
        return (s8)dst != terminator;
    }
    inline void helper(BS &bs, const char *data) {
        //doCrazyThings(acc, acc);
        //s32 tmp1 = 0;
        // = 0;
        //char tmpValue2;
        
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
        
        s32 term = 0;
        s32 &terminator = term;

        u8 tt;
        //u8 &ttt = tt;
        //const stupidididid useless; 
        while(gg(tt, data, term)) {
            //if(gg(tt, data, term, useless)) break;
            //terminator = *data - *data;
            //s32 tmp2 = tmp1;
            //tmp1 = acc + 1;
            //tmp1 = tmp2;
          //  doCrazyThings(tmp1, tmp1);
            //unimportant = *data;
            //terminator = 0;
            //tt = *data;
            //ttt = *data;
            //s32 tmp = ;
            //acc = 0;
            data++;
            acc = (s8)tt + acc * 31;
            doCrazyThings(terminator, terminator);
            //tt++;
            //acc += tmp;
            //tt = *data;
            //acc += tmp;
        }
        return acc;
    }
    
}
