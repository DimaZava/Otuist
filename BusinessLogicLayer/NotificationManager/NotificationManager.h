#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include <chrono>
#include <functional>
#include <iostream>

class NotificationManager
{
public:
    NotificationManager();
};


class Timer
{
public:
    Timer(std::function<void()> callback, int delay);
    void start();
    void stop();
    void run();

private:
    std::function<void()> callback;
    int delay;
    bool running = false;
    std::chrono::steady_clock::time_point startTime;

    void checkElapsedTime();
};

#endif // NOTIFICATIONMANAGER_H
