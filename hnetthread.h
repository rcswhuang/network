#pragma once
#include <QThread>
#include "hnetworkdef.h"
class HNetManager;
class HNetThread :public QThread
{
	Q_OBJECT
public:
	HNetThread(QObject* parent = 0);
	~HNetThread();
public:
	virtual void run();
public:
    HNetManager* m_pNetManager;
};


