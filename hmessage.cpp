#include "hmessage.h"

HMessage::HMessage()
{
    pMessage = NULL;
    wLength = 0;
}

HMessage::~HMessage()
{
    if(pMessage)
    {
        delete[] pMessage;
        pMessage = NULL;
    }
    wLength = 0;
}

bool HMessage::setMessage(char* pMessage,ushort wLength)
{
    if(!pMessage || wLength == 0)
        return false;
    this->pMessage = new char[wLength];
    if(!pMessage)
        return false;
    memcpy(this->pMessage,pMessage,wLength);
    this->wLength = wLength;
    return true;
}

char* HMessage::getMessage(ushort length)
{
    return NULL;
}


//////////////////////////////////HMessageList////////////////////////////////////
HMessageList::HMessageList()
{

}

HMessageList::~HMessageList()
{

}

void HMessageList::clearAll()
{
    while(!pMessageList.isEmpty())
        delete (HMessage*)pMessageList.takeFirst();
}

bool HMessageList::addMessage(char* pMessage,ushort wLength)
{
    if(!pMessage)
        return false;
    HMessage* pMess = new HMessage;
    if(!pMess)
        return false;
    if(!pMess->setMessage(pMessage,wLength))
    {
        delete pMess;
        pMess = NULL;
        return false;
    }
    pMessageList.append(pMess);
    return true;
}

bool HMessageList::isEmpty()
{
    return pMessageList.isEmpty();
}

HMessage* HMessageList::getHead()
{
    if(pMessageList.isEmpty())
        return NULL;
    return (HMessage*)pMessageList.first();
}

void HMessageList::removeHead()
{
    if(!pMessageList.isEmpty())
    {
        pMessageList.removeFirst();
    }
}
