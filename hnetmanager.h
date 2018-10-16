#include "hnetworkdef.h"
#include "hnetthread.h"
#include "htcpserver.h"
class HTcpServer;
class QTimerEvent;
class HTcpConnect;

class HNetManager:public QObject
{
    Q_OBJECT

public:
    //static HNetworkApp* instance();
    //static HNetworkApp* pInstance;
    //static void exitinstance();
public:
    HNetManager();
    ~HNetManager();

public:
    std::map<int,HTcpConnectPtr> conn_map;
    HTcpServer* m_tcpServerA; //服务器A网
    HTcpServer* m_tcpServerB; //服务器B网
    int m_send_mode;

public:
    void config();
    bool start();
    void run();
    void stop();

    void handle_send(char* pData,int nLength);
    void handle_recv(char* pData,int nLength);

public:
    void proc_timer();//处理定时函数
    void handle_timer(const boost::system::error_code& err);//时间处理函数

protected:
      void timerEvent(QTimerEvent *event); //定时处理函数
private:
      void start_timer();
private:
      NETCONFIG m_NetConfig;
      boost::shared_ptr<boost::asio::deadline_timer> m_p_timer;
      ptr_work work;
      asio::io_service ios;

};

