#include "PCH.hpp"
#include "Console.hpp"

Console::Console()
    : mConsoleAllocated(false)
    , mConsoleHandle(INVALID_HANDLE_VALUE)
    , mStdinHandle(INVALID_HANDLE_VALUE)
    , mStdoutHandle(INVALID_HANDLE_VALUE)
{
}

Console::~Console()
{
    if (mConsoleHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(mConsoleHandle);
        mConsoleHandle = INVALID_HANDLE_VALUE;
    }

    if (mConsoleAllocated)
    {
        FreeConsole();
        mConsoleAllocated = false;
    }
}

bool Console::Create()
{
    if (!AllocConsole())
    {
        return false;
    }

    mConsoleAllocated = true;

    mConsoleHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, 0, CONSOLE_TEXTMODE_BUFFER, 0);
    mStdinHandle = GetStdHandle(STD_INPUT_HANDLE);
    mStdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (mConsoleHandle == INVALID_HANDLE_VALUE || mStdinHandle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    if (!SetConsoleActiveScreenBuffer(mConsoleHandle))
    {
        return false;
    }

    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.dwFontSize.X = 8;
    fontInfo.dwFontSize.Y = 8;
    wcsncpy(fontInfo.FaceName, L"Terminal", 9);
    if (!SetCurrentConsoleFontEx(mStdoutHandle, false, &fontInfo))
    {
        DWORD err = GetLastError();
        std::stringstream ss;
        ss << "Failed to set console font: " << err << "\n";
        OutputDebugString(ss.str().c_str());
    }

    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(mStdoutHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(mConsoleHandle, &cursorInfo);

    return true;
}

void Console::SetPosition(uint32_t x, uint32_t y)
{
    SetConsoleCursorPosition(mConsoleHandle, {static_cast<SHORT>(x), static_cast<SHORT>(y)});
}

bool Console::GetScreenSize(uint32_t& x, uint32_t& y)
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(mConsoleHandle, &info))
    {
        return false;
    }

    x = info.dwSize.X;
    y = info.dwSize.Y;
    return true;
}

void Console::Write(const std::string& str)
{
    DWORD charsWritten = 0;
    if (!WriteConsole(mConsoleHandle, str.c_str(), static_cast<DWORD>(str.size()), &charsWritten, 0))
    {
        // log
        OutputDebugString("Failed to write\n");
    }

    if (charsWritten != str.size())
    {
        //log
        OutputDebugString("Wrote less\n");
    }
}

void Console::Write(char c)
{
    DWORD charsWritten = 0;
    if (!WriteConsole(mConsoleHandle, &c, 1, &charsWritten, 0))
    {
        // log
        OutputDebugString("Failed to write char\n");
    }
}

void Console::Clear()
{
    uint32_t x = 0, y = 0;
    GetScreenSize(x, y);

    SetPosition(0, 0);
    for (uint32_t j = 0; j < y; ++j)
        for (uint32_t i = 0; i < x; ++i)
            Write(' ');
}

void Console::SetEventCallback(EventCallback callback)
{
    mEventCallback = callback;
}

void Console::EventLoop()
{
    INPUT_RECORD event;
    DWORD eventsRead = 0;
    if (!PeekConsoleInput(mStdinHandle, &event, 1, &eventsRead))
    {
        //log
        return;
    }

    if (eventsRead > 0)
    {
        ReadConsoleInput(mStdinHandle, &event, 1, &eventsRead);
        mEventCallback(&event);
    }
}
