/** 
 * @file cohttp.cpp
 * @brief 
 * @author yingx
 * @date 2023-11-29
 */

#include <iostream>
#include "coro/task.h"
#include "coro/io_context.h"
#include "coro/awaiters.h"
#include "coro/socketawaiters.h"
#include "coro/socket.h"
#include "cohttp.h"
#include "handler.h"

task<> CoHttp::coroRequestToServer(Socket& http, std::string& buffer, HandlerNode* httphandler) {
    auto t = co_await http.connect(ip_, port_);
    if (t == 0) {
        auto s = co_await http.send((void*)buffer.c_str(), buffer.length());

        if (s > 0) {
            char data[1024*10];
            auto r = co_await http.recv(data, sizeof(data));
            data_ = data;
            httphandler->SetHttpStatus(this, 200, data_);
        } else {
            data_ = "";
            httphandler->SetHttpStatus(this, 500, data_);
        }
    } else {
        data_ = "";
        httphandler->SetHttpStatus(this, 504, data_);
    }
}

void CoHttp::requestToServer(HandlerNode* httphandler) {
    //IoContext& iocontext = IoContext::Instance();
    //Socket* http = new Socket{iocontext};
    auto c = coroRequestToServer(http_, request_, httphandler);
    c.resume();
}
