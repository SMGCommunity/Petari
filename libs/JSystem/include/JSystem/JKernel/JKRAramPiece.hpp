#pragma once

class JKRAMCommand;
class JKRAramBlock;

class JKRAramPiece {
public:
    static void prepareCommand(int, unsigned long, unsigned long, unsigned long, JKRAramBlock*, void*(unsigned long));
    static void sendCommand(JKRAMCommand*);
    static void orderAsync(int, unsigned long, unsigned long, unsigned long, JKRAramBlock*, void*(unsigned long));
    static void sync(JKRAMCommand*, int);
    static void orderSync(int, unsigned long, unsigned long, unsigned long, JKRAramBlock*);
    static void doneDMA(unsigned long);
    static void startDMA(JKRAMCommand*);
};