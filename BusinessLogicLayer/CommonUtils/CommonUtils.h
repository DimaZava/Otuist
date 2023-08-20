#ifndef COMMONUTILS_H
#define COMMONUTILS_H

#include <QDateTime>
#include <QSettings>
#include <chrono>

typedef std::chrono::time_point<std::chrono::system_clock> DateTime;

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

DateTime beginOfDate(DateTime date = now);
DateTime endOfDate(DateTime date = now);
DateTime nextHourToday(DateTime date);

DateTime stdChronoTimePointFromQDateTime(QDateTime date);
DateTime stdChronoTimePointFromQDate(QDate date);

QDateTime qDateTimeFromStdChrono(DateTime timePoint);
std::string stringFromStdChrono(DateTime date);

bool isSameDay(DateTime lhs, DateTime rhs);

bool isBetweenDates(DateTime desiredEvent, DateTime beginDate, DateTime endDate);
} // namespace Time
}; // namespace CommonUtils

#endif // COMMONUTILS_H
