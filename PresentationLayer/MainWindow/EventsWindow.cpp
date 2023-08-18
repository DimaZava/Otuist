#include "EventsWindow.h"
#include "./ui_EventsWindow.h"

#include "../../BusinessLogicLayer/CommonUtils/CommonUtils.h"
#include "../InterfaceUtils.h"

#include <QGuiApplication>
#include <QPalette>
#include <QScreen>
#include <QStyle>

EventsWindow::EventsWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::EventsWindow)
    , calendarsManager(std::make_shared<CalendarsManager>())
    , mainContentLayout(std::make_unique<QSplitter>(parent))
    , eventsContentLayout(std::make_unique<QSplitter>(mainContentLayout.get()))
    , calendarsScrollWidget(std::make_unique<CalendarsScrollWidget>(calendarsManager, mainContentLayout.get()))
    , calendarWidget(std::make_unique<CalendarWidget>(calendarsManager))
    , eventsScrollWidget(
          std::make_unique<EventsScrollWidget>(calendarsManager, *calendarWidget, eventsContentLayout.get()))
{
    ui->setupUi(this);
    configureLayout();
    setupInitialState();
    calendarsManager->reloadEvents();
}

EventsWindow::~EventsWindow()
{
    qDebug() << __PRETTY_FUNCTION__;
    delete ui;
}

void EventsWindow::configureLayout()
{
    setMinimumSize(InterfaceUtils::minimumEventsWindowSize);

    setCentralWidget(mainContentLayout.get());
    mainContentLayout->setOrientation(Qt::Orientation::Horizontal);
    mainContentLayout->addWidget(calendarsScrollWidget.get());

    eventsContentLayout->setContentsMargins(InterfaceUtils::defaultMargins);
    eventsContentLayout->setOrientation(Qt::Orientation::Vertical);
    eventsContentLayout->addWidget(calendarWidget.get());
    eventsContentLayout->addWidget(eventsScrollWidget.get());

    mainContentLayout->addWidget(eventsContentLayout.get());
}

void EventsWindow::setupInitialState()
{
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
