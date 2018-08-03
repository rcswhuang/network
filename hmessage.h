#ifndef HMESSAGE_H
#define HMESSAGE_H

#include <QObject>

class HMessage : public QObject
{
public:
    HMessage();
    ~HMessage();
private:
    char* pMessage;
    ushort wLength;

public:
    char* message(){return pMessage;}
    ushort length(){return wLength;}
    bool setMessage(char* pMessage,ushort wLength);
    void setLength(ushort length){wLength = length;}
    char* getMessage(ushort length);
};

class HMessageList
{
public:
    HMessageList();
    ~HMessageList();
private:
    QList<HMessage*> pMessageList;

public:
    void clearAll();
    bool addMessage(char* pMessage,ushort wLength);
    bool isEmpty();
    HMessage* getHead();
    void removeHead();
};


#endif // HMESSAGE_H
