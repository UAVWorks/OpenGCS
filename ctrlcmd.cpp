#include "ctrlcmd.h"

CtrlCmd::CtrlCmd(int EngineLevel, int Direction1, int Direction2, int Direction3, int FunctionBits, int ControlMode)
{
    gEngineLevel = EngineLevel;
    gDirection1 = Direction1;
    gDirection2 = Direction2;
    gDirection3 = Direction3;
    gFunctionBits = FunctionBits;
    gControlMode = ControlMode;
}
