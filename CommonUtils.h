#ifndef COMMONUTILS_H
#define COMMONUTILS_H

#include <QDateTime>
#include <QSettings>

namespace CommonUtils {
QSettings getAppSettings();
void performSetup();
std::chrono::time_point<std::chrono::system_clock> stdChronoTimePointFromQDateTime(QDateTime date);
}; // namespace CommonUtils

#endif // COMMONUTILS_H
