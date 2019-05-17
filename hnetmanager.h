#include "hnetworkdef.h"
#include "hnetthread.h"
#include "htcpserver.h"
class HTcpServer;
class QTimerEvent;
class HTcpConnect;
class HNetThread;
class HNetManager:public QObject
{
    Q_OBJECT
public:
    HNetManager(HNetThread* thread);
    ~HNetManager();

public:
    HTcpServer* m_tcpServerA; //服务器A网
    HTcpServer* m_tcpServerB; //服务器B网
    int m_send_mode;

public:
    void config();
    bool start();

    void handle_send(char* pData,int nLength);
    void handle_send(SndData*& sendData);
    void handle_recv(RecvData*& recvData);

public:
    void proc_timer();//处理定时函数
    void handle_timer(const boost::system::error_code& err);//时间处理函数
private:
      void start_timer();
private:
      HNetThread* m_pNetThread;
      NETCONFIG m_NetConfig;
      boost::shared_ptr<boost::asio::deadline_timer> m_p_timer;
      ptr_work work;
      asio::io_service ios;

      //单线程
//	  boost::shared_ptr<boost::thread>		    m_pNetManagerThread;

      //线程池
      std::vector<boost::shared_ptr<boost::thread>> m_listThread;
      std::size_t m_nThreads;

};

