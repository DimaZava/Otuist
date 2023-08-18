#include "NotificationManager.h"

NotificationManager::NotificationManager()
{

}

Timer::Timer(std::function<void()> callback, int delay)
    : callback(callback)
    , delay(delay)
{}

void Timer::start()
{
    startTime = std::chrono::steady_clock::now();
    running = true;
}

void Timer::stop()
{
    running = false;
}

void Timer::run()
{
    while (running)
    {
        checkElapsedTime();
    }
}

void Timer::checkElapsedTime()
{
    auto elapsedTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();

    if (elapsedTime >= delay)
    {
        callback();
        running = false;
    }
}
