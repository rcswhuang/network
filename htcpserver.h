#ifndef HSERVER_H
#define HSERVER_H

#define MAX_MSG_BUFFER 1024
#include "hnetworkdef.h"
#include "htcpconnect.h"
//
class HNetManager;

/*
 * 简单tcp通讯方式，服务器端双网，1对1连接通讯
*/
class HTcpServer
{
public:
    HTcpServer(io_service& io,std::string string_ip, int port);
    ~HTcpServer();
    void start(HNetManager* netManager);
    void run();
    void stop();
    int getip();

public:
    void init_accept();
    void handle_accept(const boost::system::error_code & code,HTcpConnectPtr conn);
    void handle_send(char* pData,size_t len);
public:
    HNetManager* m_pNetManager;
private:
    HTcpConnectPtr m_connects;
    ptr_acceptor m_ptrAcceptor;
    tcp::endpoint m_endpoint;
    io_service &m_ios;

};


#endif // HSERVER_H
