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
   bool   WFNetInit();
   void   WFNetEnd();
   void   sendLockAll(ushort wStationIndex,uchar btLockType);
   void   sendLockOne(ushort wStationIndex,uchar btYxIndex,uchar btLockType);

public:
    HNetHandle();
    HNetMonitor* pNetMonitor;
};

#endif // HNETHANDLE_H
