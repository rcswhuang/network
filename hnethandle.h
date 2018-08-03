#ifndef HNETHANDLE_H
#define HNETHANDLE_H

#include "publicdata.h"
#include "hnetmonitor.h"
class HNetHandle
{
public:
    static HNetHandle* instance();
    static HNetHandle* pInstance;

public:
    bool  netInit();
    void  netExit();
    void  sendWfYk(ushort wStationIndex,ushort wYxIndex, uchar btExceptValue);
    void  sendReqYk(ushort wStationIndex,ushort wYxIndex,uchar btExceptValue);
    void  sendReqAllYx(ushort wStationIndex = (ushort)-1,ushort wYxIndex = (ushort)-1);
    void  sendYkCheckBack(ushort wStationIndex,ushort wYxIndex,uchar btValue,bool bCheckStatus);
    void  sendLockAll(ushort wStationIndex,uchar btLockType);
    void  sendLockOne(ushort wStationIndex,ushort wYxIndex,uchar btLockType);
    void  sendMeasureYx(ushort* wStationIndex,ushort *wYxIndex,ushort* btYxValue,ushort wYxNum);

    ushort  stationIndex2Addr(ushort wStationIndex);
    ushort  stationAddr2Index(ushort wStaitonAddr);

public:
    HNetHandle();
    //直接用HNetMonitor netMonitor对象
    HNetMonitor* pNetMonitor;
};

#endif // HNETHANDLE_H
