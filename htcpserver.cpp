#include "htcpserver.h"
#include "htcpconnect.h"
#include "hnetmanager.h"

extern void add_msg_for_show(unsigned short type,std::string msg);
//////////////////////////////////////////HServer///////////////////////////////////
HTcpServer::HTcpServer(io_service& io,std::string ip, int port):m_ios(io),m_endpoint(ip::address::from_string(ip),port)
{
    m_ptrAcceptor = boost::shared_ptr<tcp::acceptor>(new tcp::acceptor(m_ios,m_endpoint));
    m_ptrAcceptor->set_option(tcp::acceptor::reuse_address(true));
}

HTcpServer::~HTcpServer()
{
    //创建的连接全部析构掉
    stop();
}

void HTcpServer::start(HNetManager* manager)
{
    m_pNetManager = manager;
    //m_ptrAcceptor->listen();
    init_accept();
}

void HTcpServer::run()
{
    //m_ios.run();
}

void HTcpServer::stop()
{
    m_ptrAcceptor->close();
    if(m_connects)
    {
        m_connects->stop();
        m_connects.reset();
    }
}

int HTcpServer::getip()
{
    return m_endpoint.address().to_v4().to_ulong();
}

void HTcpServer::init_accept()
{
    HTcpConnectPtr connect = HTcpConnectPtr(new HTcpConnect(m_ios));
    m_ptrAcceptor->async_accept(connect->sokcet(), boost::bind(&HTcpServer::handle_accept, this,asio::placeholders::error,connect));
}

void HTcpServer::handle_accept(const boost::system::error_code & ec,HTcpConnectPtr pTcpClientPtr)
{
    if (ec)
    {
        //输出打印日志
        //std::ostringstream os;
        //os.str().reserve(256);
        return;
    }

    ip::tcp::socket& socket_ = pTcpClientPtr->sokcet();
    std::ostringstream os;
    os.str().reserve(128);
    os << "Accept a new connection:	" << socket_.remote_endpoint();
    add_msg_for_show(MSG_INFORMATION,os.str());
    m_connects = pTcpClientPtr;
    init_accept();
    pTcpClientPtr->start(m_pNetManager);

}

//处理发送
void HTcpServer::handle_send(char* pbuf, size_t bytes)
{

    {
    }
}
