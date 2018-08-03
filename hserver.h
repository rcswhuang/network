#ifndef HSERVER_H
#define HSERVER_H

#define MAX_MSG_BUFFER 1024
<<<<<<< HEAD
#include "networkapi.h"
#include "htcpclient.h"
=======
#include "hnetworkapi.h"
>>>>>>> df21d31b704e7de76870272105708fa357e85b11
//
class HNetworkApp;

/*
 * 简单tcp通讯方式，服务器端双网，1对1连接通讯
*/
class HTcpServer
{
public:
    HTcpServer(io_service& io,std::string string_ip, int port,HNetworkApp* app);
public:
    void start();
    void run();
    void stop();
    int getip();

    HTcpClientPtr find_tcp_client_ptr(int ip);
public:
    void init_accept();
<<<<<<< HEAD
    void handle_accept(const boost::system::error_code & code,HTcpClientPtr conn);
    void handle_send(char* pData,size_t len);
=======
    void handle_accept(const boost::system::error_code & code,ptr_connect conn);
>>>>>>> df21d31b704e7de76870272105708fa357e85b11
public:
    HNetworkApp* m_p_network;
    std::map<int,HTcpClientPtr> m_clinets;
private:
    asio::ip::tcp::acceptor m_acceptor;
    ip::tcp::endpoint m_endpoint;
    io_service &m_ios;

};


#endif // HSERVER_H
