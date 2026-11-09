#pragma once
#include <coroutine>
#include <exception>

class PlayerTask {
public:
    struct promise_type {
        std::exception_ptr exception;
        PlayerTask get_return_object();
        std::suspend_always initial_suspend() noexcept;
        std::suspend_always final_suspend() noexcept;
        void return_void() noexcept;
        void unhandled_exception() noexcept;
    };

    using Handle = std::coroutine_handle<promise_type>;
    explicit PlayerTask(Handle handle);

    PlayerTask(const PlayerTask&) = delete;
    PlayerTask& operator=(const PlayerTask&) = delete;
    PlayerTask(PlayerTask&& other) noexcept;
    PlayerTask& operator=(PlayerTask&& other) noexcept;
    ~PlayerTask();

    bool done() const;
    void resume();

private:
    void destroy();
    Handle handle;
};