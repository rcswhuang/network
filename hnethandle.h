#ifndef HNETHANDLE_H
#define HNETHANDLE_H

//#include "hkernelapi.h"
#include "hnetmonitor.h"
#include "hnetthread.h"
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
    HNetMonitor* m_pNetMonitor;
    HNetThread* m_pNetThread;
};

#endif // HNETHANDLE_H
