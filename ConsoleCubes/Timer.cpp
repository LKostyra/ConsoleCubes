#include "PCH.hpp"
#include "Timer.hpp"

Timer::Timer()
{
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    mFreq = static_cast<double>(freq.QuadPart);
    QueryPerformanceCounter(&mStart);
}

Timer::~Timer()
{
}

void Timer::Start()
{
    QueryPerformanceCounter(&mStart);
}

double Timer::Stop()
{
    LARGE_INTEGER stop;
    QueryPerformanceCounter(&stop);
    return static_cast<double>(stop.QuadPart - mStart.QuadPart) / mFreq;
}
