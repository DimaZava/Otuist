#include "EventsWindow.h"
#include "./ui_EventsWindow.h"

#include "CommonUtils.h"
#include "InterfaceUtils.h"

#include <QGuiApplication>
#include <QPalette>
#include <QScreen>
#include <QStyle>

EventsWindow::EventsWindow(std::shared_ptr<CalendarsRepository> calendarsRepository, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::EventsWindow)
    , mainContentLayout(std::unique_ptr<QSplitter>{new QSplitter{parent}})
    , foldersScrollWidget(std::unique_ptr<CalendarsScrollWidget>{new CalendarsScrollWidget{mainContentLayout.get()}})
    , calendar(std::unique_ptr<EventsCalendarWidget>{new EventsCalendarWidget})
    , calendarsRepository(calendarsRepository)
{
    ui->setupUi(this);
    configureLayout();
    setupInitialState();
}

EventsWindow::~EventsWindow()
{
    mainContentLayout.release();
    foldersScrollWidget.release();
    calendar.release();
    delete ui;
}

void EventsWindow::configureLayout()
{
    setMinimumSize(InterfaceUtils::minimumEventsWindowSize);

    setCentralWidget(mainContentLayout.get());
    mainContentLayout->addWidget(foldersScrollWidget.get());

    calendar.get()->setContentsMargins(InterfaceUtils::defaultMargins);
    mainContentLayout->addWidget(calendar.get());
}

void EventsWindow::setupInitialState()
{
    // const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
    // const std::chrono::system_clock::time_point tomorrow = now + std::chrono::hours(100 * 24);

    // Event event11 = Event{now, tomorrow, "event11", "some description 11"};
    // event11.setCategories("Work");
    // Event event12 = Event{now, tomorrow, "event12", "some description 12"};
    // event12.setCategories("Home");
    // Event event13 = Event{now, tomorrow, "event13", "some description 13"};
    // event13.setCategories("Family");

    // qDebug() << event11.getName() << " " << event11.getCategory().value_or("n/a");
    // qDebug() << event12.getName() << " " << event12.getCategory().value_or("n/a");
    // qDebug() << event13.getName() << " " << event13.getCategory().value_or("n/a");
    foldersScrollWidget->setCalendarItems(calendarsRepository->getCalendars());
}

void EventsWindow::writePositionSizeSettings()
{
    QSettings appSettings = CommonUtils::getAppSettings();
    appSettings.beginGroup(metaObject()->className());

    appSettings.setValue("geometry", saveGeometry());
    appSettings.setValue("savestate", saveState());
    appSettings.setValue("maximized", isMaximized());

    if (!isMaximized())
    {
        appSettings.setValue("pos", pos());
        appSettings.setValue("size", size());
    }

    appSettings.endGroup();
}

void EventsWindow::readPositionSizeSettings()
{
    QSettings appSettings = CommonUtils::getAppSettings();
    appSettings.beginGroup(metaObject()->className());

    if (!appSettings.allKeys().isEmpty())
    {
        restoreGeometry(appSettings.value("geometry", saveGeometry()).toByteArray());
        restoreState(appSettings.value("savestate", saveState()).toByteArray());
        move(appSettings.value("pos", pos()).toPoint());
        resize(appSettings.value("size", size()).toSize());
        if (appSettings.value("maximized", isMaximized()).toBool())
            showMaximized();
    }
    else
    {
        QScreen* screen = QGuiApplication::primaryScreen();
        QSize screenSize = screen->availableSize();
        setGeometry((screenSize.width() / 2) - (InterfaceUtils::minimumEventsWindowSize.width() / 2),
                    (screenSize.height() / 2)
                        - (InterfaceUtils::minimumEventsWindowSize.height() / 2),
                    InterfaceUtils::minimumEventsWindowSize.width(),
                    InterfaceUtils::minimumEventsWindowSize.height());
    }

    appSettings.endGroup();
}

void EventsWindow::moveEvent(QMoveEvent *)
{
    writePositionSizeSettings();
}

void EventsWindow::resizeEvent(QResizeEvent *)
{
    writePositionSizeSettings();
}

void EventsWindow::closeEvent(QCloseEvent *)
{
    writePositionSizeSettings();
}
