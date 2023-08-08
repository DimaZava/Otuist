#include "CalendarsLayout.h"
#include "InterfaceUtils.h"

#include <QHeaderView>
#include <set>

CalendarsLayout::CalendarsLayout(QWidget* parent)
    : QVBoxLayout(parent)
    , categoriesTree(std::unique_ptr<QTreeWidget>(new QTreeWidget))
{
    configureLayout();
}

CalendarsLayout::~CalendarsLayout()
{
    categoriesTree.release();
}

void CalendarsLayout::configureLayout()
{
    categoriesTree->setHeaderHidden(true);
    categoriesTree.get()->setHeaderLabels({"Is Enabled", "Title"});
    categoriesTree.get()->header()->setSectionResizeMode(QHeaderView::Stretch);

    categoriesTree->setContentsMargins(InterfaceUtils::zeroMargins);
    categoriesTree->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    categoriesTree->setColumnWidth(0, 25);
    // categoriesTree->setColumnWidth(1, 100);
    addWidget(categoriesTree.get());
}

void CalendarsLayout::setCalendarItems(const std::set<std::shared_ptr<CalendarItem>>& calendarItems)
{
    this->calendarItems = calendarItems;
    reloadData();
}

void CalendarsLayout::reloadData()
{
    categoriesTree->clear();

    for (const auto& calendarItem : calendarItems)
    {
        QTreeWidgetItem* topWidgetItem = new QTreeWidgetItem{categoriesTree.get()};
        topWidgetItem->setText(0, calendarItem.get()->getName().c_str());

        for (const auto& category : calendarItem.get()->getCategories())
        {
            QTreeWidgetItem* categoryWidgetItem = new QTreeWidgetItem{topWidgetItem};
            categoryWidgetItem->setCheckState(0, Qt::CheckState::Checked);
            auto tmp = category.c_str();
            qDebug() << tmp;
            categoryWidgetItem->setText(1, category.c_str());
        }
    }
}
