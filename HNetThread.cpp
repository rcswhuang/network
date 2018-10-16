#pragma once
#include "hnetthread.h"
#include "hnetmanager.h"

HNetThread::HNetThread(QObject* parent)
{
	
}
HNetThread::~HNetThread()
{
    delete m_pNetManager;
    wait();
}

void HNetThread::run()
{
    m_pNetManager = new HNetManager;
    if(m_pNetManager->start())
        m_pNetManager->run();
}
