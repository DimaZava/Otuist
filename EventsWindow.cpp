#include "EventsWindow.h"
#include "./ui_EventsWindow.h"

#include <QGuiApplication>
#include <QScreen>
#include <QStyle>
#include <QTextEdit> // tmp usage, remove if not needed
#include "CommonUtils.h"
#include "InterfaceUtils.h"

EventsWindow::EventsWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EventsWindow)
    , mainContentLayout(std::unique_ptr<QSplitter>{new QSplitter{parent}})
    , foldersScrollArea(std::unique_ptr<QScrollArea>{new QScrollArea{mainContentLayout.get()}})
    , foldersLayoutContainer(std::unique_ptr<QWidget>{new QWidget{foldersScrollArea.get()}})
    , foldersLayout(std::unique_ptr<QVBoxLayout>{new QVBoxLayout{foldersLayoutContainer.get()}})
    , calendar(std::unique_ptr<QCalendarWidget>{new QCalendarWidget})
{
    ui->setupUi(this);
    configureLayout();
}

EventsWindow::~EventsWindow()
{
    mainContentLayout.release();
    foldersScrollArea.release();
    foldersLayoutContainer.release();
    foldersLayout.release();
    calendar.release();
    delete ui;
}

void EventsWindow::configureLayout()
{
    setMinimumSize(InterfaceUtils::minimumEventsWindowSize);

    QTextEdit *textedit1 = new QTextEdit;
    textedit1->setFixedHeight(150);
    QTextEdit *textedit2 = new QTextEdit;
    textedit2->setFixedHeight(150);
    QTextEdit *textedit3 = new QTextEdit;
    textedit3->setFixedHeight(150);
    QTextEdit *textedit4 = new QTextEdit;
    textedit4->setFixedHeight(150);

    setCentralWidget(mainContentLayout.get());

    foldersLayoutContainer->setLayout(foldersLayout.get());

    foldersScrollArea->setWidgetResizable(true);
    foldersScrollArea->setWidget(foldersLayoutContainer.get());
    foldersScrollArea->setMinimumWidth(200);
    foldersScrollArea->setMaximumWidth(300);
    mainContentLayout->addWidget(foldersScrollArea.get());

    foldersLayout->setSizeConstraint(QLayout::SetMaximumSize);
    foldersLayout->addWidget(textedit1);
    foldersLayout->addWidget(textedit2);
    foldersLayout->addWidget(textedit3);
    foldersLayout->addWidget(textedit4);
    foldersLayout->setContentsMargins(InterfaceUtils::defaultMargins);

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
