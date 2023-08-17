#include "CommonUtils.h"
#include "../../BusinessLogicLayer/CommonUtils/CommonUtils.h"

#include <QCoreApplication>
#include <iomanip>
#include <sstream>

QSettings CommonUtils::getAppSettings()
{
    return QSettings();
}

void CommonUtils::performSetup()
{
    QCoreApplication::setOrganizationName("dimazava@otus");
    QCoreApplication::setOrganizationDomain("otus.ru");
    QCoreApplication::setApplicationName("Otuist");
}

std::chrono::time_point<std::chrono::system_clock> CommonUtils::Time::stdChronoTimePointFromQDateTime(QDateTime date)
{
    std::tm time = {};
    std::istringstream ss{date.toString(qDateTimeFormat).toStdString()};
    ss >> std::get_time(&time, dateTimeFormat);
    auto utc_time_point = std::chrono::system_clock::from_time_t(std::mktime(&time));

    std::time_t utc_time_t = std::chrono::system_clock::to_time_t(utc_time_point);
    std::tm* local_tm = std::localtime(&utc_time_t);
    auto local_time_point = std::chrono::system_clock::from_time_t(std::mktime(local_tm));

    return local_time_point;
}

std::chrono::time_point<std::chrono::system_clock> CommonUtils::Time::stdChronoTimePointFromQDate(QDate date)
{
    std::tm time = {};
    std::istringstream ss{date.toString(qDateFormat).toStdString()};
    ss >> std::get_time(&time, dateFormat);
    auto utc_time_point = std::chrono::system_clock::from_time_t(std::mktime(&time));

    std::time_t utc_time_t = std::chrono::system_clock::to_time_t(utc_time_point);
    std::tm* local_tm = std::localtime(&utc_time_t);
    auto local_time_point = std::chrono::system_clock::from_time_t(std::mktime(local_tm));

    return local_time_point;
}

QDateTime CommonUtils::Time::qDateTimeFromStdChrono(std::chrono::time_point<std::chrono::system_clock> timePoint)
{
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(timePoint.time_since_epoch()).count();
    return QDateTime::fromSecsSinceEpoch(duration);
}

std::string CommonUtils::Time::stringFromStdChrono(std::chrono::time_point<std::chrono::system_clock> date)
{
    std::time_t tt = std::chrono::system_clock::to_time_t(date);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&tt), dateTimeFormat);
    return ss.str();
}

std::chrono::time_point<std::chrono::system_clock> CommonUtils::Time::beginOfDate(
    std::chrono::time_point<std::chrono::system_clock> date)
{
    auto current_date = std::chrono::floor<std::chrono::days>(date);
    auto beginning_of_day = current_date.time_since_epoch();
    auto time_point = std::chrono::time_point<std::chrono::system_clock>{beginning_of_day};
    return time_point;
}

std::chrono::time_point<std::chrono::system_clock> CommonUtils::Time::endOfDate(
    std::chrono::time_point<std::chrono::system_clock> date)
{
    std::time_t current_time_t = std::chrono::system_clock::to_time_t(date);
    std::tm* current_date = std::localtime(&current_time_t);
    current_date->tm_hour = 23;
    current_date->tm_min = 59;
    current_date->tm_sec = 59;

    auto endOfDay = std::chrono::system_clock::from_time_t(std::mktime(current_date));

    return endOfDay;
}

bool CommonUtils::Time::isSameDay(
    std::chrono::time_point<std::chrono::system_clock> lhs,
    std::chrono::time_point<std::chrono::system_clock> rhs)
{
    std::time_t lhs_current_time_t = std::chrono::system_clock::to_time_t(lhs);
    std::tm* lhs_current_date = std::localtime(&lhs_current_time_t);

    std::time_t rhs_current_time_t = std::chrono::system_clock::to_time_t(rhs);
    std::tm* rhs_current_date = std::localtime(&rhs_current_time_t);

    return lhs_current_date->tm_year == rhs_current_date->tm_year &&
        lhs_current_date->tm_mon == rhs_current_date->tm_mon && lhs_current_date->tm_mday == rhs_current_date->tm_mday;
}

bool CommonUtils::Time::isBetweenDates(
    std::chrono::time_point<std::chrono::system_clock> desiredEvent,
    std::chrono::time_point<std::chrono::system_clock> beginDate,
    std::chrono::time_point<std::chrono::system_clock> endDate)
{
    return beginDate < endDate && beginDate <= desiredEvent && desiredEvent <= endDate;
}
