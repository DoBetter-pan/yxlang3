/** 
 * @file io_context.h
 * @brief 
 * @author yingx
 * @date 2023-11-23
 */

#pragma once

#include <set>

class Socket;
class Conn;
class Send;
class Recv;

class IoContext {
public:
    IoContext();
    ~IoContext();
    void run();
    void stop();
    static IoContext& Instance() {
        static IoContext instance;
        return instance;
    }
private:
    friend Socket;
    friend Conn;
    friend Send;
    friend Recv;

    constexpr static std::size_t max_events = 10;
    int fd_;
    bool quit_;

    void Attach(Socket* socket);
    void WatchRead(Socket* socket);
    void UnwatchRead(Socket* socket);
    void WatchWrite(Socket* socket);
    void UnwatchWrite(Socket* socket);
    void Detach(Socket* socket);
};
