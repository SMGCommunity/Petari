#pragma once

#include <revolution.h>

class JKRAMCommand;
class JKRAramBlock;

class JKRAramPiece {
public:
    static void prepareCommand(int, unsigned long, unsigned long, unsigned long, JKRAramBlock*, void*(unsigned long));
    static void sendCommand(JKRAMCommand*);
    static void orderAsync(int, unsigned long, unsigned long, unsigned long, JKRAramBlock*, void*(unsigned long));
    static void sync(JKRAMCommand*, int);
    static BOOL orderSync(int, unsigned long, unsigned long, unsigned long, JKRAramBlock*);
    static void doneDMA(unsigned long);
    static void startDMA(JKRAMCommand*);
};

inline void JKRAramPcs_SendCommand(JKRAMCommand* command) {
    JKRAramPiece::sendCommand(command);
}

inline BOOL JKRAramPcs(int direction, u32 source, u32 destination, u32 length, JKRAramBlock* block) {
    return JKRAramPiece::orderSync(direction, source, destination, length, block);
}
