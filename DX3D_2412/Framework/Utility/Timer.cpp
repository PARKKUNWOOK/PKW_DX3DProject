#include "Framework.h"

Timer::Timer()
{
    //1초동안 CPU진동수를 반환하는 함수
    QueryPerformanceFrequency((LARGE_INTEGER*)&periodFrquency);

    //현재 CPU진동수를 반환하는 함수
    QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);

    timeScale = 1.0f / (float)periodFrquency;
}

Timer::~Timer()
{
}

void Timer::Update()
{
    QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
    elapsedTime = (float)(curTime - lastTime) * timeScale;

    lastTime = curTime;

    totalTime += elapsedTime;

    frameCount++;
    oneSecCount += elapsedTime;

    if (oneSecCount >= 1.0f)
    {
        frameRate = frameCount;
        frameCount = 0;
        oneSecCount = 0.0f;
    }
}