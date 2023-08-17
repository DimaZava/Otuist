#ifndef INTERFACEUTILS_H
#define INTERFACEUTILS_H

#include <QMargins>
#include <QMessageBox>
#include <QSize>

namespace InterfaceUtils {
static const QSize minimumEventsWindowSize = QSize(800, 500);
static const QSize minimumAddEventDialogSize = QSize(385, 328);
static const QMargins defaultMargins = QMargins(8, 8, 8, 8);
static const QMargins zeroMargins = QMargins(0, 0, 0, 0);

int showConfirmationAlert(
    QString text,
    QString informativeText,
    QMessageBox::StandardButtons standardButtons,
    QMessageBox::StandardButton defaultButton);
}; // namespace InterfaceUtils

#endif // INTERFACEUTILS_H
