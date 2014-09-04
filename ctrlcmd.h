#ifndef CTRLCMD_H
#define CTRLCMD_H

class CtrlCmd
{
public:
    CtrlCmd(int EngineLevel, int Direction1, int Direction2, int Direction3, int FunctionBits, int ControlMode);
    int gEngineLevel;
    int gDirection1;
    int gDirection2;
    int gDirection3;
    int gFunctionBits;
    int gControlMode;
};

#endif // CTRLCMD_H
