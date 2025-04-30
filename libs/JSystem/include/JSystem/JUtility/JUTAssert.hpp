#pragma once

#include <revolution.h>

class JUTAssertion {
public:
    static void create();
    static void flush_subroutine();
    static void flushMessage();
    static void flushMessage_dbPrint();
    static void changeDisplayTime(u32);
    static void changeDevice(u32);
    static void setVisible(bool);
};
