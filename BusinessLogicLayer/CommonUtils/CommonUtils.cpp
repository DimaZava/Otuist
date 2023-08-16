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
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&time));
    return tp;
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
    return std::chrono::time_point<std::chrono::system_clock>{beginning_of_day};
}

std::chrono::time_point<std::chrono::system_clock> CommonUtils::Time::endOfDate(
    std::chrono::time_point<std::chrono::system_clock> date)
{
    auto current_date = std::chrono::floor<std::chrono::days>(date);
    auto tomorrow = current_date + std::chrono::hours(24);
    auto endOfDay = tomorrow - std::chrono::seconds(1);
    return std::chrono::time_point<std::chrono::system_clock>{endOfDay};
}
