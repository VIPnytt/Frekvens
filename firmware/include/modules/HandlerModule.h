#pragma once

class HandlerModule
{
public:
    HandlerModule() = default;
    virtual ~HandlerModule() = default;

    HandlerModule(const HandlerModule &) = delete;
    HandlerModule &operator=(const HandlerModule &) = delete;
    HandlerModule(HandlerModule &&) = delete;
    HandlerModule &operator=(HandlerModule &&) = delete;
};
