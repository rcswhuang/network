#ifndef HNETWORKDEF_H
#define HNETWORKDEF_H
#include <set>
#include <thread>
#include <atomic>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/xpressive/xpressive_dynamic.hpp>
#include <boost/typeof/typeof.hpp>
using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::posix_time;
using namespace boost::interprocess;
using namespace boost::xpressive;

typedef unsigned int service_id;
typedef boost::system::error_code err_code;
typedef boost::shared_ptr<ip::tcp::socket> ptr_socket;
typedef boost::shared_ptr<ip::tcp::acceptor> ptr_acceptor;
typedef boost::shared_ptr<io_service::work> ptr_work;


typedef void(*Callback)(unsigned int msg_typd, const std::string& msg, const unsigned int msg_length);
typedef void(*TCPServerCallback)(unsigned int msg_typd, const std::string& msg, const unsigned int msg_length);

#define TCP_SERVER_NUM 1
/****************************************************************************/
typedef struct _tagNetConfig
{
    std::string str_ip_master;
    std::string str_ip_reserver;
    int port;
}NETCONFIG;

/****************************************************************************************************************/
//消息结构
//注意不同的规约头结构不一致，要根据规约更改头结构。
#define HEAD_SIZE   4  //头长度
#define DATA_SIZE  4096 //报文长度
/*
typedef struct _tagHead
{
    unsigned char  funNo;
    unsigned short length;
}HEAD;*/

typedef struct _tagHead
{
    //unsigned char  funNo;
    unsigned int length;
}HEAD;

typedef struct _tagData
{
    HEAD head;
    char* data;
}DATA;
#define HEADERLEN sizeof(HEADER)

typedef struct _tagnetData
{
    char* data;
    int len;
    uint64_t ipport;
    //int ip;
    //std::string time;//发送时间
}RecvData,SndData,ShowData;

/**************************************************************************************************************/
//消息显示设置
#define SHOWMSGLEN 100
#define MAXSHOWMSG 300

#define MSG_NULL_TYPE 0
#define MSG_R_TYPE 1
#define MSG_W_TYPE 2

#define MSG_LINK_SEND 0x80 //链路接收
#define MSG_LINK_RECV 0x81 //链路发送
#define MSG_LINK_ERROR 0x82 //链路层关闭等错误

#define MSG_APP_SEND  0x90 //规约层发报文
#define MSG_APP_RECV  0x91 //规约层收报文
#define MSG_APP_ERROR 0x92 //规约层错误报文

#define MSG_INFORMATION 0x100
#define MSG_WARNNING 0x0101
#define MSG_ERROR 0x102


#define TYPE_MSG_RECV_YX 0x01
#define TYPE_MSG_RECV_YC 0x02
#define TYPE_MSG_RECV_YK 0x03

#define TYPE_MSG_SEND_HEART        0x10
#define TYPE_MSG_SEND_CHECK_OK     0x11
#define TYPE_MSG_SEND_CHECK_FAILED 0x12

#define TYPE_MSG_ERROR 0x100

typedef struct _tagShowMsg
{
    unsigned short type;//消息类型
    char* data;//报文数据
    int len; //报文长度
    //int ip; //发送地点
    //std::string time;//发送时间
    std::string info;//消息文字
}ShowMsg;

/*****************************************************************************************************************/
//双网模式，1 主备模式；2 双主模式
#define MODE_MASTER_RESERVER 0x01
#define MODE_DOUBLE_MASTER   0x02



enum _tagConnectStatus
{
    INITCONNECT = 0x00,
    CONNECTING,
    CONNECTED,
    CLOSING = 0xF0,
    CLOSED = 0xFF
};

typedef struct _tagMsg
{

}INTERMSG;

//define net tree
#define TREE_NET_NULL        0x100
#define TREE_NET_ALL         0x101
#define TREE_NET_A           0x102
#define TREE_NET_B           0x103

#define TREE_PROTOCOL_ALL    0x110
#define TREE_PROTOCOL_A      0x111
#define TREE_PROTOCOL_B      0x112

#endif // NETWORKAPI_H
