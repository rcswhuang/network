#pragma once
#include "hnetthread.h"
#include "hnetmanager.h"
#include "hprotocol.h"
HNetThread::HNetThread()
{
    m_pNetManager = NULL;
    m_pProtocol = NULL;
}
HNetThread::~HNetThread()
{
    if(m_pNetManager)
        delete m_pNetManager;
    if(m_pProtocol)
        delete m_pProtocol;
}

void HNetThread::start()
{
    m_pNetManager = new HNetManager(this);
    m_pNetManager->start();
    m_pProtocol = new HProtocol;
    m_pProtocol->start();
}
