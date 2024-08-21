/** 
 * @file awaiters.h
 * @brief 
 * @author yingx
 * @date 2023-11-23
 */

#pragma once

#include <iostream>
#include <coroutine>

template<typename T> struct task;

namespace yxlang {
template<typename T>
struct promise_type_base {
    std::coroutine_handle<> continuation_ = std::noop_coroutine();
    task<T> get_return_object();
    std::suspend_always initial_suspend() {
        return {};
    }

    struct final_awaiter {
        bool await_ready() noexcept {
            return false;
        }
        void await_resume() noexcept {
        }

        template<typename promise_type>
        std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> coro) noexcept {
            return coro.promise().continuation_;
        }
    };
    auto final_suspend() noexcept {
        return final_awaiter{};
    }
    void unhandled_exception() {
        // std::exit(-1);
    }
};

template<typename T>
struct promise_type final : promise_type_base<T> {
    T result;
    void return_value(T value) {
        result = value;
    }
    T await_resume() {
        return result;
    }
    task<T> get_return_object();
};

template<>
struct promise_type<void> final : promise_type_base<void> {
    void return_void() {
    }
    void await_resume() {
    }
    task<void> get_return_object();
};

}

template<typename T = void>
struct task {
    using promise_type = yxlang::promise_type<T>;
    task() : handle_(nullptr) {
    }
    task(std::coroutine_handle<promise_type> handle) : handle_(handle) {
    }
    bool await_ready() {
        return false;
    }
    T await_resume() {
        return handle_.promise().result;
    }
    void await_suspend(std::coroutine_handle<> waiter) {
        handle_.promise().continuation_ = waiter;
        handle_.resume();
    }
    void resume() {
        handle_.resume();
    }

    std::coroutine_handle<promise_type> handle_;
};

namespace yxlang {
template<typename T>
inline task<T> promise_type<T>::get_return_object() {
    return task<T>{ std::coroutine_handle<promise_type<T>>::from_promise(*this) };
}

inline task<void> promise_type<void>::get_return_object() {
    return task<void>{ std::coroutine_handle<promise_type<void>>::from_promise(*this) };
}
}
