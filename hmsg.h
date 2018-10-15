#ifndef HMSG_H_
#define HMSG_H_
#include "hnetworkdef.h"
class HMsg
{
public:
    enum
    {
        header_length = HEAD_SIZE, //头长度
        max_data_length = DATA_SIZE, //报文长度
        max_message_length = HEAD_SIZE + DATA_SIZE //整个
    };
    HMsg()
    {
        data_len = 0;
    }

    void clear();

    char *msg();
    std::size_t msg_length();

    char* data();
    std::size_t data_length();
    void set_data_length(std::size_t length);

    bool analy_header();

    bool analy_nr_header(HEAD* phead);

private:


    char msg_[HEAD_SIZE + DATA_SIZE];
    std::size_t data_len;
};
#endif






























