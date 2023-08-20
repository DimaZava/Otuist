#include "NotificationManager.h"

#include <QThread>
#include <QtCore>

// NotificationManager

NotificationManager::NotificationManager()
{}

void NotificationManager::scheduleNotification(const SharedCalendarEvent& event)
{
    // Naive

    // auto notificationClosure = [&event, this]() {
    //     auto updatedScript =
    //         QString("osascript -e 'display notification \"message\" with title\"Event started\" subtitle \"%1\"'")
    //             .arg(event->getName().c_str());
    //     system(updatedScript.toStdString().c_str());

    // if (auto it = scheduledNotifications.find(event->getUid()); it != scheduledNotifications.end())
    //     scheduledNotifications.erase(it);
    // };
    // auto timer = std::make_shared<Timer>(notificationClosure, 10000);

    // Future based

    // std::future<void> future = std::async(std::launch::async | std::launch::deferred, [this, &event]() {
    //     auto storedTimer = std::get<1>(*(&scheduledNotifications.at(event->getUid())));
    //     storedTimer->start();
    //     storedTimer->run();
    // });

    // scheduledNotifications[event->getUid()] = std::make_tuple<std::future<void>, std::shared_ptr<Timer>>(
    //     std::async(
    //         std::launch::async,
    //         [this, &event]() {
    //     auto storedTimer = std::get<1>(*(&scheduledNotifications.at(event->getUid())));
    //     storedTimer->start();
    //     storedTimer->run();
    //         }),
    //     std::make_shared<Timer>(notificationClosure, 10000));

    // auto future = &std::get<0>(*(&scheduledNotifications.at(event->getUid())));
    // future->wait();

    // Promise based

    // std::promise<void> notificationPromise;
    // std::future<void> notificationFuture = notificationPromise.get_future();

    // std::thread promiseThread(&NotificationManager::_scheduleNotification, this, std::move(notificationPromise),
    // event);

    // notificationFuture.wait();
    // promiseThread.join();

    // Qt native implementation
    QThread* backgroundThread = new QThread;
    this->moveToThread(backgroundThread);
    connect(backgroundThread, &QThread::started, this, [&event, this]() {
        auto notificationClosure = [event, this]() {
            auto updatedScript =
                QString("osascript -e 'display notification \"message\" with title\"Event started\" subtitle \"%1\"'")
                    .arg(event->getName().c_str());
            system(updatedScript.toStdString().c_str());

            if (auto it = scheduledNotificationsTimers.find(event->getUid()); it != scheduledNotificationsTimers.end())
                scheduledNotificationsTimers.erase(it);
        };
        // 15 Seconds for demonstration purposes, update to production value once needed
        auto timer = std::make_shared<Timer>(notificationClosure, 15000);
        scheduledNotificationsTimers[event->getUid()] = timer;

        timer->start();
        timer->run();
    });
    backgroundThread->start();
}

void NotificationManager::unscheduleNotification(const SharedCalendarEvent& event)
{
    if (auto it = scheduledNotificationsTimers.find(event->getUid()); it != scheduledNotificationsTimers.end())
    {
        it->second->stop();
        scheduledNotificationsTimers.erase(it);
    }
}

// Timer

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
    stop();
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
