/** 
 * @file cohttp.h
 * @brief 
 * @author yingx
 * @date 2023-11-29
 */

#pragma once

#include <iostream>
#include "coro/task.h"
#include "coro/io_context.h"
#include "coro/awaiters.h"
#include "coro/socketawaiters.h"
#include "coro/socket.h"

class HandlerNode;

class CoHttp {
public:
    CoHttp() : http_(IoContext::Instance()), ip_("127.0.0.1"), port_(80)   {
    }
    CoHttp(const std::string& ip, int port) : http_(IoContext::Instance()), ip_(ip), port_(port) {
    }
    ~CoHttp() {
    }
    
    task<int> connectToServer(Socket& http, std::string& buffer, HandlerNode* httphandler);
    task<> coroRequestToServer(Socket& http, std::string& buffer, HandlerNode* httphandler);
    void requestToServer(HandlerNode* httphandler);
    void setRequestData(std::string& buffer) {
        request_ = buffer;
    }
    std::string& getData() {
        return data_;
    }

private:
    Socket http_;
    std::string ip_;
    int port_;
    std::string request_;
    std::string data_;
};
