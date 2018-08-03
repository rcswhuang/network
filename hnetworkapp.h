#include "hnetworkapi.h"
#include "hnetthread.h"
#include "hserver.h"
class HTcpServer;
class QTimerEvent;
class HTcpClient;
/*
 * 本网络仅对双网且是主备模式
*/
class HNetworkApp:public QObject
{
    Q_OBJECT

public:
    static HNetworkApp* instance();
    static HNetworkApp* pInstance;
    static void exitinstance();
public:
    HNetworkApp();
    ~HNetworkApp();

public:
    std::map<int,HTcpClientPtr> conn_map;
    HTcpServer* m_tcpServerA; //服务器A网
    HTcpServer* m_tcpServerB; //服务器B网
    int m_send_mode;

public:
    //void setNetThread(HNetThread* pThread);
    void readOption();
    void start();
    void run();
    void stop();

    void handle_send(char* pData,int nLength);
    void handle_send(RecvData* recvdata);
    //统一改成定时互斥处理，单客户端，多客户端均适用
    void handle_recv(char* pData,int nLength,int ip,std::string& time);
    void proc_recv_data();

public:
    void proc_timer();//处理定时函数
    void handle_timer(const boost::system::error_code& err);//时间处理函数

protected:
      void timerEvent(QTimerEvent *event); //定时处理函数
private:
      void start_timer();
private:
    boost::shared_ptr<boost::asio::deadline_timer> m_p_timer;
    ptr_work work;
    //HNetThread* pNetThread;
    asio::io_service ios;

};

