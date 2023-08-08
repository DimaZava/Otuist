#include "CommonUtils.h"
#include "InterfaceUtils.h"

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

std::chrono::time_point<std::chrono::system_clock> CommonUtils::stdChronoTimePointFromQDateTime(QDateTime date)
{
    std::tm tm = {};
    std::stringstream ss{date.toString(InterfaceUtils::qDateFormat).toStdString()};
    ss >> std::get_time(&tm, InterfaceUtils::dateFormat);
    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    return tp;
}
