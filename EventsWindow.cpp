#include "EventsWindow.h"
#include "./ui_EventsWindow.h"

#include "CommonUtils.h"
#include "InterfaceUtils.h"
#include <QGuiApplication>
#include <QScreen>
#include <QStyle>

EventsWindow::EventsWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::EventsWindow)
    , mainContentLayout(std::unique_ptr<QSplitter>{new QSplitter{parent}})
    , foldersScrollWidget(std::unique_ptr<FoldersScrollWidget>{new FoldersScrollWidget{mainContentLayout.get()}})
    , calendar(std::unique_ptr<QCalendarWidget>{new QCalendarWidget})
{
    ui->setupUi(this);
    configureLayout();
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
