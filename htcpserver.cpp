#include "htcpserver.h"
#include "hconnect.h"
#include "hnetworkapp.h"
//////////////////////////////////////////HServer///////////////////////////////////
HTcpServer::HTcpServer(io_service& io,std::string ip, int port,HNetworkApp *app):m_ios(io),m_endpoint(ip::address::from_string(ip),port),
  m_p_network(app),m_acceptor(m_ios,m_endpoint)
{

}

void HTcpServer::start()
{
    m_acceptor.listen();
    init_accept();
}

void HTcpServer::run()
{
    //m_ios.run();
}

void HTcpServer::stop()
{
    m_acceptor.close();
}

int HTcpServer::getip()
{
    return m_endpoint.address().to_v4().to_ulong();
}

void HTcpServer::init_accept()
{
    HConnectPtr connect = HConnectPtr(new HConnect(m_ios,m_p_network));
    m_acceptor.async_accept(connect->sokcet(), boost::bind(&HTcpServer::handle_accept, this,asio::placeholders::error,connect));
}

void HTcpServer::handle_accept(const boost::system::error_code & ec,HConnectPtr pClientPtr)
{
    if (!ec)
    {
        //输出打印日志
        std::ostringstream os;
        os.str().reserve(256);
        return;
    }

    ip::tcp::socket& socket_ = pClientPtr->sokcet();
    std::ostringstream os;
    os.str().reserve(128);
    os << "Accept a new connection:	" << socket_.remote_endpoint()
        << "	==>	" << socket_.local_endpoint() << "\n";
    int ip = pClientPtr->getip();
    //m_clients[ip] = pClientPtr;
    m_p_network->conn_map[ip] = pClientPtr;
    init_accept();
    pClientPtr->start();

}

HConnectPtr HTcpServer::find_tcp_connect_ptr(int ip)
{
    std::map<int,HConnectPtr>::const_iterator it = m_connects.find(ip);
    while(it != m_connects.end())
    {
        return m_connects[ip];
    }
    return nullptr;
}


//处理发送
void HTcpServer::handle_send(char* pbuf, size_t bytes)
{

    {
    }
}
