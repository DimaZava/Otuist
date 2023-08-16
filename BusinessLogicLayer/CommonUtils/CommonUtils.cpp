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
    std::tm tm = {};
    std::stringstream ss{date.toString(qDateTimeFormat).toStdString()};
    ss >> std::get_time(&tm, dateTimeFormat);
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));

    return tp;
}

std::chrono::time_point<std::chrono::system_clock> CommonUtils::Time::stdChronoTimePointFromQDate(QDate date)
{
    std::tm time = {};
    std::istringstream ss{date.toString(qDateFormat).toStdString()};
    ss >> std::get_time(&time, dateFormat);
    qDebug() << ss.str();
    auto utc_time_point = std::chrono::system_clock::from_time_t(std::mktime(&time));

    std::time_t utc_time_t = std::chrono::system_clock::to_time_t(utc_time_point);
    std::tm* local_tm = std::localtime(&utc_time_t);
    auto local_time_point = std::chrono::system_clock::from_time_t(std::mktime(local_tm));

    qDebug() << CommonUtils::Time::stringFromStdChrono(local_time_point);
    return local_time_point;
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
    qDebug() << CommonUtils::Time::stringFromStdChrono(time_point);
    return time_point;
}

std::chrono::time_point<std::chrono::system_clock> CommonUtils::Time::endOfDate(
    std::chrono::time_point<std::chrono::system_clock> date)
{
    qDebug() << CommonUtils::Time::stringFromStdChrono(date);
    std::time_t current_time_t = std::chrono::system_clock::to_time_t(date);
    std::tm* current_date = std::localtime(&current_time_t);
    current_date->tm_hour = 23;
    current_date->tm_min = 59;
    current_date->tm_sec = 59;

    auto endOfDay = std::chrono::system_clock::from_time_t(std::mktime(current_date));
    qDebug() << CommonUtils::Time::stringFromStdChrono(endOfDay);

    return endOfDay;
}
