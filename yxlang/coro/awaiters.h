/** 
 * @file awaiters.h
 * @brief 
 * @author yingx
 * @date 2023-11-23
 */

#pragma once

#include <unistd.h>
#include <sys/types.h>
#include <memory>
#include <iostream>
#include <type_traits>
#include <coroutine>

template<typename Syscall, typename ReturnValue>
class AsyncSyscall {
public:
    AsyncSyscall() : suspended_(false), called_(false) {
    }

    bool await_ready() const noexcept {
        return false;
    }

    bool await_suspend(std::coroutine_handle<> h) noexcept {
        static_assert(std::is_base_of_v<AsyncSyscall, Syscall>);
        handle_ = h;
        //static_cast<Syscall*>(this)->SetCoroHandle();
        value_ = static_cast<Syscall*>(this)->Syscall();
        suspended_ = (value_ == -1 && (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINPROGRESS));
        if (suspended_) {
            static_cast<Syscall*>(this)->SetCoroHandle();
        }
        return suspended_;
    }

    ReturnValue await_resume() noexcept {
        /*
        if (suspended_ && called_) {
            value_ = static_cast<Syscall*>(this)->Syscall();
        }
        return value_;
        */
        value_ = static_cast<Syscall*>(this)->ResumeSyscall();
        return value_;
    }

protected:
    std::coroutine_handle<> handle_;
    bool suspended_;
    bool called_;
    ReturnValue value_;
};
