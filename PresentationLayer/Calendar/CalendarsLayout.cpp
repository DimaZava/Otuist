#include "CalendarsLayout.h"
#include "../../PresentationLayer/InterfaceUtils.h"

#include <QHeaderView>
#include <set>

enum class WidgetCalendarColumns
{
    categoryName = 0
};

enum class WidgetCategoryColumns
{
    checkState = 0,
    categoryName
};

CalendarsLayout::CalendarsLayout(const std::shared_ptr<CalendarsRepository>& calendarsRepository, QWidget* parent)
    : QVBoxLayout(parent)
    , calendarsRepository(calendarsRepository)
    , categoriesTree(std::make_unique<QTreeWidget>(new QTreeWidget))
{
    configureLayout();
}

CalendarsLayout::~CalendarsLayout()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void CalendarsLayout::configureLayout()
{
    categoriesTree->setHeaderHidden(true);
    categoriesTree->setHeaderLabels({"Is Enabled", "Title"});
    categoriesTree->header()->setSectionResizeMode(QHeaderView::Stretch);

    categoriesTree->setContentsMargins(InterfaceUtils::zeroMargins);
    categoriesTree->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    categoriesTree->setColumnWidth(0, 25);
    // categoriesTree->setColumnWidth(1, 100);

    connect(categoriesTree.get(), &QTreeWidget::itemChanged, this, &CalendarsLayout::treeItemDidChange);
    addWidget(categoriesTree.get());

    reloadData();
    categoriesTree->expandAll();
}

void CalendarsLayout::reloadData()
{
    categoriesTree->clear();

    for (const auto& calendarItem : calendarsRepository->getCalendars())
    {
        QTreeWidgetItem* topWidgetItem = new QTreeWidgetItem{categoriesTree.get()};
        topWidgetItem->setText(0, calendarItem.get()->getName().c_str());

        for (const auto& category : calendarItem.get()->getCategories())
        {
            QTreeWidgetItem* categoryWidgetItem = new QTreeWidgetItem{topWidgetItem};
            categoryWidgetItem->setCheckState(
                static_cast<int>(WidgetCategoryColumns::checkState), Qt::CheckState::Checked);
            categoryWidgetItem->setText(static_cast<int>(WidgetCategoryColumns::categoryName), category->name.c_str());
        }
    }
}

void CalendarsLayout::treeItemDidChange(QTreeWidgetItem* item, int column)
{
    QTreeWidgetItem* calendarTreeWidgetItem = item->parent();
    if (calendarTreeWidgetItem == nullptr)
        return;

    std::string calendarName =
        calendarTreeWidgetItem->text(static_cast<int>(WidgetCalendarColumns::categoryName)).toStdString();
    bool newCheckState = item->checkState(static_cast<int>(WidgetCategoryColumns::checkState));
    std::string categoryName = item->text(static_cast<int>(WidgetCategoryColumns::categoryName)).toStdString();
    calendarsRepository->setCalendarsCategoryActive(calendarName, categoryName, newCheckState);
}
