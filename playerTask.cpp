#include "playerTask.h"

PlayerTask PlayerTask::promise_type::get_return_object() {
    return PlayerTask(Handle::from_promise(*this));
}

std::suspend_always PlayerTask::promise_type::initial_suspend() noexcept {
    return {};
}

std::suspend_always PlayerTask::promise_type::final_suspend() noexcept {
    return {};
}

void PlayerTask::promise_type::return_void() noexcept {
}

void PlayerTask::promise_type::unhandled_exception() noexcept {
    exception = std::current_exception();
}

PlayerTask::PlayerTask(Handle handle)
    : handle(handle) {
}

PlayerTask::PlayerTask(PlayerTask&& other) noexcept
    : handle(other.handle) {
    other.handle = nullptr;
}

PlayerTask& PlayerTask::operator=(PlayerTask&& other) noexcept {
    if (this != &other) {
        destroy();
        handle = other.handle;
        other.handle = nullptr;
    }
    return *this;
}

PlayerTask::~PlayerTask() {
    destroy();
}

bool PlayerTask::done() const {
    return !handle || handle.done();
}

void PlayerTask::resume() {
    if (done()) {
        return;
    }
    handle.resume();
    if (handle.promise().exception) {
        std::rethrow_exception(handle.promise().exception);
    }
}

void PlayerTask::destroy() {
    if (handle) {
        handle.destroy();
        handle = nullptr;
    }
}