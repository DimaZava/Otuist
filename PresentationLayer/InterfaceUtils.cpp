#include "InterfaceUtils.h"

int InterfaceUtils::showConfirmationAlert(
    QString text,
    QString informativeText,
    QMessageBox::StandardButtons standardButtons,
    QMessageBox::StandardButton defaultButton)
{
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.setInformativeText(informativeText);
    msgBox.setStandardButtons(standardButtons);
    msgBox.setDefaultButton(defaultButton);
    return msgBox.exec();
}
