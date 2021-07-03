#include "JKernel/JKRDecomp.h"
#include "JKernel/JKRHeap.h"

JKRDecomp* JKRDecomp::create(s32 a1)
{
    if (!JKRDecomp::sDecompObject)
    {
        JKRDecomp::sDecompObject = new(JKRHeap::sSystemHeap, 0)JKRDecomp(a1);
    }

    return JKRDecomp::sDecompObject;
}

JKRDecomp::JKRDecomp(s32 a1) : JKRThread(0x4000, 0x10, a1)
{
    OSResumeThread(mThread);
}

JKRDecomp::~JKRDecomp()
{

}

s32 JKRDecomp::run()
{
    OSInitMessageQueue(&JKRDecomp::sMessageQueue, &JKRDecomp::sMessageBuffer, 8);

    while (1)
    {
        OSMessage msg;

        OSReceiveMessage(&JKRDecomp::sMessageQueue, &msg, 1);

        JKRDecompCommand* cmd = static_cast<JKRDecompCommand*>(msg);
        JKRDecomp::decode(cmd->_4, cmd->_8, cmd->_C, cmd->_10);

        if (cmd->_20 != 0)
        {
            if (cmd->_20 == 1)
            {
                JKRAramPiece::sendCommand(cmd->_24);
            }

            continue;
        }

        if (cmd->_14)
        {
            cmd->_14((u32)cmd);
            continue;
        }

        if (cmd->_1C)
        {
            OSSendMessage(cmd->_1C, (OSMessage)1, 0);
        }
        else
        {
            OSSendMessage(&cmd->_28, (OSMessage)1, 0);
        }
        
    }
}

JKRDecompCommand* JKRDecomp::prepareCommand(u8 *a1, u8 *a2, u32 a3, u32 a4, JKRDecompCommand::AsyncCallback cb)
{
    JKRDecompCommand* cmd = new(JKRHeap::sSystemHeap, -4)JKRDecompCommand();
    cmd->_4 = a1;
    cmd->_8 = a2;
    cmd->_C = a3;
    cmd->_10 = a4;
    cmd->_14 = cb;

    return cmd;
}

void JKRDecomp::sendCommand(JKRDecompCommand *pCmd)
{
    OSSendMessage(&JKRDecomp::sMessageQueue, pCmd, 0);
}

bool JKRDecomp::sync(JKRDecompCommand *pCmd, int type)
{
    OSMessage msg;
    bool ret;

    if (type == 0)
    {
        OSReceiveMessage(&pCmd->_28, &msg, 1);
        ret = 1;
    }
    else
    {
        ret = OSReceiveMessage(&pCmd->_28, &msg, 0);
    }
    
    return ret;
}

bool JKRDecomp::orderSync(u8 *a1, u8 *a2, u32 a3, u32 a4)
{
    JKRDecompCommand* cmd = JKRDecomp::prepareCommand(a1, a2, a3, a4, 0);
    OSSendMessage(&JKRDecomp::sMessageQueue, cmd, 0);
    bool sync = JKRDecomp::sync(cmd, 0);
    
    if (cmd)
    {
        delete cmd;
    }

    return sync;
}

void JKRDecomp::decode(u8 *pIn, u8 *pOut, u32 unk1, u32 unk2)
{
    s32 decompType = checkCompressed(pIn);

    if (decompType == 1)
    {
        decodeSZP(pIn, pOut, unk1, unk2);
    }
    else if (decompType == 2)
    {
        decodeSZS(pIn, pOut, unk1, unk2);
    }
}

void JKRDecomp::decodeSZS(u8 *pIn, u8 *pOut, u32 unk1, u32 unk2)
{
    s32 loopVal = 0;
    u32 curBlock;
    s32 totalSize = unk2 - ((s32)pOut + *(u32*)(pIn + 4));

    if (unk1 == 0)
    {
        return;
    }

    if (unk2 > *(u32*)pIn)
    {
        return;
    }

    pIn += 0x10;

    do
    {
        if (loopVal == 0)
        {
            curBlock = *pIn;
            loopVal = 8;
            pIn++;
        }

        if ((curBlock & 0x80) != 0)
        {
            if (unk2 == 0)
            {
                unk1--;
                *pOut++ = *pIn;

                if (unk1 == 0)
                {
                    return;
                }
            }
            else
            {
                unk2--;
            }

            pIn++;
        }
        else
        {
            u8 val1 = *pIn++;
            u8 val2 = *pIn++;

            s32 numBytes = val1 >> 4;
            u8* copyLoc = (u8*)pOut;
            copyLoc -= ((val1 & 0xF) << 8) | val2;

            if (numBytes == 0)
            {
                numBytes = *pIn++ + 0x12;
            }
            else
            {
                numBytes += 0x2;
            }
            
            do
            {
               if (unk2 == 0)
                {
                    unk1--;
                    *pOut = *(copyLoc - 1);
                    pOut++;

                    if (unk1 == 0)
                    {
                        return;
                    }
                }
                else
                {
                    unk2--;
                }

                copyLoc--;

            } while (--numBytes != 0);
        }

        curBlock <<= 1;
        loopVal--;

    } while ((s32)pOut != totalSize);
}

s32 JKRDecomp::checkCompressed(u8 *pSrc)
{
    // Return values:
    // 0 -- Not Compressed
    // 1 -- Yay0
    // 2 -- Yaz0
    // 3 -- ASR
    if (pSrc[0] == 0x59 && pSrc[1] == 0x61 && pSrc[3] == 0x30)
    {
        if (pSrc[2] == 0x79)
        {
            return 1;
        }

        if (pSrc[2] == 0x7A)
        {
            return 2;
        }
    }

    if (pSrc[0] == 0x41 && pSrc[1] == 0x53 && pSrc[2] == 0x52)
    {
        return 3;
    }
    
    return 0;
}

JKRDecompCommand::JKRDecompCommand()
{
    OSInitMessageQueue(&_28, &_48, 1);
    _18 = this;
    _14 = 0;
    _1C = 0;
    _20 = 0;
}