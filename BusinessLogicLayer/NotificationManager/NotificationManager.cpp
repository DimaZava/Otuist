#include "NotificationManager.h"

#include <QtCore>

NotificationManager::NotificationManager()
{
}

void NotificationManager::scheduleNotification(SharedCalendarEvent event)
{
    auto notificationClosure = [&event]() {
        auto updatedScript =
            QString("osascript -e 'display notification \"message\" with title\"Attention\" subtitle \"%1\"'")
                .arg(event->getName().c_str());
        system(updatedScript.toStdString().c_str());
    };
    auto timer = std::make_unique<Timer>(notificationClosure, 150);
    timer->start();
    timer->run();
    scheduledNotifications[event->getUid()] = std::move(timer);
}

Timer::Timer(std::function<void()> callback, int delay)
    : callback(callback)
    , delay(delay)
{}

Timer::Timer(Timer&& timer) noexcept
    : callback(std::exchange(timer.callback, nullptr))
    , delay(timer.delay)
    , running(timer.running)
    , startTime(timer.startTime)
{}

Timer::~Timer()
{
    qDebug() << __PRETTY_FUNCTION__;
}

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

Timer& Timer::operator=(Timer&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }

    callback = std::exchange(other.callback, nullptr);
    delay = other.delay;
    running = other.running;
    startTime = other.startTime;
    return *this;
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
