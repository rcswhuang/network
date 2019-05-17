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
    init_accept();
}

void HTcpServer::run()
{

}

void HTcpServer::stop()
{
    m_ptrAcceptor->close();
    map<uint64_t, HTcpConnectPtr>::iterator i;
    for (i = m_connects.begin(); i != m_connects.end(); ++i)
    {
        HTcpConnectPtr p = i->second;
        p.reset();
    }
    m_connects.clear();
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

    uint32_t ip = socket_.remote_endpoint().address().to_v4().to_ulong();
    uint16_t port = (uint16_t)socket_.remote_endpoint().port();
    uint64_t ipport = ((uint64_t)ip << 16) | (uint64_t)(port);

    m_connects[ipport] = pTcpClientPtr;
    pTcpClientPtr->m_pTcpServer = this;

    init_accept();
    pTcpClientPtr->start(m_pNetManager);
}

HTcpConnectPtr HTcpServer::findConnect(uint64_t ipport)
{
    map<uint64_t, HTcpConnectPtr>::const_iterator it = m_connects.find(ipport);
    if (it != m_connects.end())
    {
        return m_connects[ipport];
    }
    return nullptr;
}

void HTcpServer::removeConnect(uint64_t ipport)
{
    map<uint64_t, HTcpConnectPtr>::iterator it = m_connects.find(ipport);
    if (it != m_connects.end())
    {
        HTcpConnectPtr pClient = m_connects[ipport];
        m_connects.erase(it);
        pClient.reset();
    }
}

//处理发送
void HTcpServer::handle_send(char* pbuf, size_t bytes)
{

    {
    }
}
