/** 
 * @file socket.h
 * @brief 
 * @author yingx
 * @date 2023-11-23
 */

#pragma once

#include <unistd.h>
#include <iostream>
#include <memory>
#include <coroutine>
#include "task.h"
#include <stdexcept>

class Conn;
class Send;
class Recv;
class IoContext;

class Socket {
public:
    Socket(IoContext& io_context);
    Socket(int fd, IoContext& io_context);
    Socket(const Socket&) = delete;
    Socket(Socket&& socket) : io_context_(socket.io_context_), fd_(socket.fd_), io_state_(socket.io_state_) {
        socket.fd_ = -1;
    }
    ~Socket();

    Conn connect(const std::string& ip, int port);
    Recv recv(void* buffer, std::size_t len);
    Send send(void* buffer, std::size_t len);

    bool ResumeRecv();
    bool ResumeSend();

private:
    friend IoContext;
    friend Conn;
    friend Send;
    friend Recv;
    
private:
    IoContext& io_context_;
    int fd_ = -1;
    int32_t io_state_ = 0;
    std::coroutine_handle<> coro_recv_;
    std::coroutine_handle<> coro_send_;
};
