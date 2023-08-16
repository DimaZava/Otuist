#include "CalendarsLayout.h"
#include "../../PresentationLayer/InterfaceUtils.h"

#include <QHeaderView>
#include <set>

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
    categoriesTree.reset();
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
            categoryWidgetItem->setCheckState(0, Qt::CheckState::Checked);
            auto tmp = category.c_str();
            qDebug() << tmp;
            categoryWidgetItem->setText(1, category.c_str());
        }
    }
}
