#include "hmsg.h"
/*
 * HMsg类有header_length,主要原因是用了async_read函数先读头结构的，如果头结构正确，获取到报文长度，则接着读报文
 * 所以要求规约头里面必须包含整个报文长度。不过现在规约基本上都是如此定义的。
*/
void HMsg::clear()
{
    memset(msg_,0,sizeof(max_message_length));
}

char *HMsg::msg()
{
    return msg_;
}

std::size_t HMsg::msg_length()
{
    return header_length + data_len;
}

char* HMsg::data()
{
    return msg_+header_length;
}

std::size_t HMsg::data_length()
{
    return data_len;
}

void HMsg::set_data_length(std::size_t length)
{
    data_len = length;
}

bool HMsg::analy_header()
{
    //if(msg_[0] == 0)
    //    return false;
    unsigned char temp[header_length];
    memset(temp,0,header_length);
    memcpy(temp,msg_,header_length);

    HEAD* pHead = (HEAD*)temp;
    int ll = ntohl((int)pHead->length);

    if(ll > max_data_length)
    {
        data_len = 0;
        return false;
    }
   // int length = atoi(temp);

    /*注意不同的头结构此处要区别判断*/
    /*if(!analy_nr_header(pHead))
    {
        data_len = 0;
        return false;
    }*/
    data_len = ll;
    return true;
}

bool HMsg::analy_nr_header(HEAD* pHead)
{
    //if(pHead->funNo < 0x85 || ((pHead->funNo > 0x88) &&(pHead->funNo != 0x91 ) && (pHead->funNo != 0x96)))
    {
   //     return false;
    }
    return true;
}
