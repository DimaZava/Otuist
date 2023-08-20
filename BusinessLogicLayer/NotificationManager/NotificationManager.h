#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include "../../ModelsLayer/CalendarEvent/CalendarEvent.h"

#include <QCoreApplication>
#include <chrono>
#include <functional>
#include <map>

class Timer
{
public:
    Timer(std::function<void()> callback, int delay);
    Timer(const Timer& timer) = delete;
    Timer(Timer&& timer) noexcept;
    ~Timer();

    void start();
    void stop();
    void run();

    Timer& operator=(const Timer& other) = delete;
    Timer& operator=(Timer&& other) noexcept;

private:
    std::function<void()> callback;
    int delay;
    bool running = false;
    std::chrono::steady_clock::time_point startTime;

    void checkElapsedTime();
};

class NotificationManager : public QObject
{
    Q_OBJECT
public:
    NotificationManager();

    void scheduleNotification(const SharedCalendarEvent& event);
    void unscheduleNotification(const SharedCalendarEvent& event);

private:
    std::map<std::string, std::shared_ptr<Timer>> scheduledNotificationsTimers;
};

#endif // NOTIFICATIONMANAGER_H
