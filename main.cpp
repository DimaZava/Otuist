#include "EventsWindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "CommonUtils.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Otuist_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    CommonUtils::performSetup();

    // TODO:
    // Replace with service locator?
    EventsWindow w{std::shared_ptr<CalendarsRepository>{new CalendarsRepository}};
    w.readPositionSizeSettings();
    w.show();
    return a.exec();
}
