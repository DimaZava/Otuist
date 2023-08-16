#ifndef CALENDARSLAYOUT_H
#define CALENDARSLAYOUT_H

#include "../../BusinessLogicLayer/CalendarsRepository/CalendarsRepository.h"

#include <QLabel>
#include <QTreeWidget>
#include <QVBoxLayout>

class CalendarsLayout : public QVBoxLayout
{
    Q_OBJECT
public:
    explicit CalendarsLayout(
        const std::shared_ptr<CalendarsRepository>& calendarsRepository,
        QWidget* parent = nullptr);
    ~CalendarsLayout() override;

private:
    const std::shared_ptr<CalendarsRepository>& calendarsRepository;
    std::unique_ptr<QTreeWidget> categoriesTree;

    void configureLayout();
    void reloadData();

private slots:
    void treeItemDidChange(QTreeWidgetItem* item, int column);
};

#endif // CALENDARSLAYOUT_H
