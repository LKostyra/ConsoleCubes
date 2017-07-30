#pragma once

class Timer
{
    double mFreq;
    LARGE_INTEGER mStart;

public:
    Timer();
    ~Timer();

    void Start();
    double Stop();
};