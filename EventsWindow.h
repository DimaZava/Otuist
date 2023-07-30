#ifndef EVENTSWINDOW_H
#define EVENTSWINDOW_H

#include <QCalendarWidget>
#include <QMainWindow>
#include <QScrollArea>
#include <QSplitter>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class EventsWindow; }
QT_END_NAMESPACE

class EventsWindow : public QMainWindow
{
    Q_OBJECT

public:
    EventsWindow(QWidget *parent = nullptr);
    ~EventsWindow();

    void readPositionSizeSettings();

private:
    Ui::EventsWindow *ui;
    std::unique_ptr<QSplitter> mainContentLayout;
    std::unique_ptr<QScrollArea> foldersScrollArea;
    std::unique_ptr<QWidget> foldersLayoutContainer;
    std::unique_ptr<QVBoxLayout> foldersLayout;
    std::unique_ptr<QCalendarWidget> calendar;

    void configureLayout();
    void writePositionSizeSettings();

    void moveEvent(QMoveEvent *event);
    void resizeEvent(QResizeEvent *event);
    void closeEvent(QCloseEvent *event);
};
#endif // EVENTSWINDOW_H
