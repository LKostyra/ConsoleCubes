#pragma once

using EventCallback = std::function<void(INPUT_RECORD*)>;

class Console
{
    bool mConsoleAllocated;
    HANDLE mConsoleHandle;
    HANDLE mStdinHandle;
    HANDLE mStdoutHandle;

    EventCallback mEventCallback;

public:
    Console();
    ~Console();

    bool Create();

    void SetPosition(uint32_t x, uint32_t y);
    bool GetScreenSize(uint32_t& x, uint32_t& y);
    void Write(const std::string& str);
    void Write(char c);
    void SetEventCallback(EventCallback callback);
    bool EventLoop();
};
