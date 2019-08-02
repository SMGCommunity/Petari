#ifndef NAMEOBJEXECUTEHOLDER_H
#define NAMEOBJEXECUTEHOLDER_H

class NameObj;

namespace MR
{
    void registerNameObjToExecuteHolder(NameObj *, s32, s32, s32, s32);
    void connectToSceneTemporarily(NameObj *);
    void disconnectToSceneTemporarily(NameObj *);
    void connectToDrawTemporarily(NameObj *);
    void disconnectToDrawTemporarily(NameObj *);
};

#endif // NAMEOBJEXECUTEHOLDER_H