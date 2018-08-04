#ifndef HSERVER_H
#define HSERVER_H

#define MAX_MSG_BUFFER 1024
#include "networkapi.h"
#include "hconnect.h"
//
class HNetworkApp;

/*
 * 简单tcp通讯方式，服务器端双网，1对1连接通讯
*/
class HTcpServer
{
public:
    HTcpServer(io_service& io,std::string string_ip, int port,HNetworkApp* app);
    void start();
    void run();
    void stop();
    int getip();

    HConnectPtr find_tcp_connect_ptr(int ip);
public:
    void init_accept();
    void handle_accept(const boost::system::error_code & code,HConnectPtr conn);
    void handle_send(char* pData,size_t len);
public:
    HNetworkApp* m_p_network;
private:
    std::map<int,HConnectPtr> m_connects;
    asio::ip::tcp::acceptor m_acceptor;
    ip::tcp::endpoint m_endpoint;
    io_service &m_ios;

};


#endif // HSERVER_H
