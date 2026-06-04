#pragma once

#include "JSystem/JSupport/JSUList.hpp"

struct JASTrack;

struct JASPortArgs {
    JASTrack* _00;
    u32 _04;
    u32 _08;
    f32 _0C;
    f32 _10;
    f32 _14;
    f32 _18;
    f32 _1C;
    f32 _20;
    u32 _24;
    f32 _28;
};

struct JASPortCmd : JSULink< JASPortCmd > {
    typedef void (*Command)(JASPortArgs*);

    struct TPortHead : JSUList< JASPortCmd > {
        inline TPortHead() : JSUList< JASPortCmd >() {
        }

        ~TPortHead() {
        }

        void execCommandOnce();
        void execCommandStay();
    };

    bool addPortCmdOnce();
    bool setPortCmd(Command func, JASPortArgs*);
    static void execAllCommand();

    Command getFunc() {
        return mFunc;
    }
    JASPortArgs* getArgs() {
        return mArgs;
    }

    Command mFunc;
    JASPortArgs* mArgs;

    static TPortHead sCommandListOnce;
    static TPortHead sCommandListStay;
};
