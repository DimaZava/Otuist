#ifndef COMMONUTILS_H
#define COMMONUTILS_H

#include <QSettings>

namespace CommonUtils {
QSettings getAppSettings();
void performSetup();
}; // namespace CommonUtils

#endif // COMMONUTILS_H
