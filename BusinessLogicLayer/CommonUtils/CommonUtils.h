#ifndef COMMONUTILS_H
#define COMMONUTILS_H

#include <QDateTime>
#include <QSettings>
#include <chrono>

namespace CommonUtils {
QSettings getAppSettings();
void performSetup();

namespace Time {
static const auto now = std::chrono::system_clock::now();
static const auto distantPast = std::chrono::system_clock::from_time_t(0);
static const char* qDateFormat = "dd.MM.yyyy";
static const char* qDateTimeFormat = "dd.MM.yyyy hh:mm";
static const char* dateFormat = "%d.%m.%Y";
static const char* dateTimeFormat = "%d.%m.%Y %H:%M";

std::chrono::time_point<std::chrono::system_clock> beginOfDate(
    std::chrono::time_point<std::chrono::system_clock> date = now);
std::chrono::time_point<std::chrono::system_clock> endOfDate(
    std::chrono::time_point<std::chrono::system_clock> date = now);

std::chrono::time_point<std::chrono::system_clock> stdChronoTimePointFromQDateTime(QDateTime date);
std::chrono::time_point<std::chrono::system_clock> stdChronoTimePointFromQDate(QDate date);

QDateTime qDateTimeFromStdChrono(std::chrono::time_point<std::chrono::system_clock> timePoint);
std::string stringFromStdChrono(std::chrono::time_point<std::chrono::system_clock> date);

bool isSameDay(
    std::chrono::time_point<std::chrono::system_clock> lhs,
    std::chrono::time_point<std::chrono::system_clock> rhs);

bool isBetweenDates(
    std::chrono::time_point<std::chrono::system_clock> desiredEvent,
    std::chrono::time_point<std::chrono::system_clock> beginDate,
    std::chrono::time_point<std::chrono::system_clock> endDate);
} // namespace Time
}; // namespace CommonUtils

#endif // COMMONUTILS_H
