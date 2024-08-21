/** 
 * @file socketawaiters.h
 * @brief 
 * @author yingx
 * @date 2023-11-23
 */

#pragma once

#include <unistd.h>
#include <cstring>
#include <memory>
#include <iostream>
#include <sys/types.h>
#include <type_traits>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include "awaiters.h"
#include "socket.h"

class Socket;

class Conn : public AsyncSyscall<Conn, ssize_t> {
public:
    Conn(Socket* socket, const std::string& ip, int port) : AsyncSyscall(), socket_(socket), ip_(ip), port_(port) {
        //socket_->io_context_.WatchWrite(socket_);
    }
    ~Conn() {
        socket_->io_context_.UnwatchWrite(socket_);
    }

    ssize_t Syscall() {
        struct sockaddr_in sockaddr;
        bzero(&sockaddr, sizeof(sockaddr));
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_addr.s_addr = inet_addr(ip_.c_str());
        sockaddr.sin_port = htons(port_);
        //return ::connect(socket_->fd_, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
        int ret = ::connect(socket_->fd_, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
        return ret;
    }

    ssize_t ResumeSyscall() {
        int32_t err = 0;
        socklen_t errlen = sizeof(err);
        int ret = getsockopt(socket_->fd_, SOL_SOCKET, SO_ERROR, (void*)&err, &errlen);
        if (ret == -1 || err != 0) {
            return -1;
        } else {
            return 0;
        }
    }

    void SetCoroHandle() {
        socket_->coro_send_ = handle_;
        socket_->io_context_.WatchWrite(socket_);
    }
private:
    Socket* socket_;
    std::string ip_;
    int port_;
};

class Send : public AsyncSyscall<Send, ssize_t> {
public:
    Send(Socket* socket, void* buffer, std::size_t len) : AsyncSyscall(), socket_(socket), buffer_(buffer), len_(len) {
        socket_->io_context_.WatchWrite(socket_);
    }
    ~Send() {
        socket_->io_context_.UnwatchWrite(socket_);
    }

    ssize_t Syscall() {
        return ::send(socket_->fd_, buffer_, len_, 0);
    }

    ssize_t ResumeSyscall() {
        return value_;
    }

    void SetCoroHandle() {
        socket_->coro_send_ = handle_;
    }
private:
    Socket* socket_;
    void* buffer_;
    std::size_t len_;
};

class Recv : public AsyncSyscall<Recv, int> {
public:
    Recv(Socket* socket, void* buffer, size_t len) : AsyncSyscall(), socket_(socket), buffer_(buffer), len_(len) {
        called_ = true;
        socket_->io_context_.WatchRead(socket_);
    }
    ~Recv() {
        socket_->io_context_.UnwatchRead(socket_);
    }

    ssize_t Syscall() {
        return ::recv(socket_->fd_, buffer_, len_, 0);
    }

    ssize_t ResumeSyscall() {
        return ::recv(socket_->fd_, buffer_, len_, 0);
    }

    void SetCoroHandle() {
        socket_->coro_recv_ = handle_;
    }
private:
    Socket* socket_;
    void* buffer_;
    std::size_t len_;
};

