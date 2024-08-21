/** 
 * @file socket.cpp
 * @brief\
 * @author yingx
 * @date 2023-11-23
 */

#include <memory>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <string_view>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include "socket.h"
#include "io_context.h"
#include "socketawaiters.h"

Socket::Socket(IoContext& io_context) : io_context_(io_context) {
    fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_ < 0) {
        throw std::runtime_error{"can not create socket"};
    }
    fcntl(fd_, F_SETFL, O_NONBLOCK);
    io_context_.Attach(this);
}

Socket::Socket(int fd, IoContext& io_context) : io_context_(io_context), fd_(fd) {
    fcntl(fd_, F_SETFL, O_NONBLOCK);
    io_context_.Attach(this);
}

Socket::~Socket() {
    if (fd_ != -1) {
        io_context_.Detach(this);
        ::close(fd_);
    }
}

Conn Socket::connect(const std::string& ip, int port) {
    return Conn(this, ip, port);
}

Recv Socket::recv(void* buffer, std::size_t len) {
    return Recv{this, buffer, len};
}

Send Socket::send(void* buffer, std::size_t len) {
    return Send{this, buffer, len};
}

bool Socket::ResumeRecv() {
    if (coro_recv_) {
        coro_recv_.resume();
        return true;
    } else {
        return false;
    }
}

bool Socket::ResumeSend() {
    if (coro_send_) {
        coro_send_.resume();
        return true;
    } else {
        return false;
    }
}
