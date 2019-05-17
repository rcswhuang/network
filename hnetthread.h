#pragma once
#include <QThread>
#include "hnetworkdef.h"
class HNetManager;
class HProtocol;
class HNetThread : public QObject
{
	Q_OBJECT
public:
    HNetThread();
	~HNetThread();
public:
    void start();
public:
    HNetManager* m_pNetManager;
    HProtocol* m_pProtocol;

};


