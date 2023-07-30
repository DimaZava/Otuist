#include "FoldersScrollWidget.h"

#include <QTextEdit> // tmp usage, remove if not needed

#include "InterfaceUtils.h"

FoldersScrollWidget::FoldersScrollWidget(QWidget* parent)
    : QScrollArea(parent)
    , foldersLayoutContainer(std::unique_ptr<QWidget>{new QWidget{this}})
    , foldersLayout(std::unique_ptr<QVBoxLayout>{new QVBoxLayout{foldersLayoutContainer.get()}})
{
    configureLayout();
}

FoldersScrollWidget::~FoldersScrollWidget()
{
    foldersLayoutContainer.release();
    foldersLayout.release();
}

void FoldersScrollWidget::configureLayout()
{
    foldersLayoutContainer->setLayout(foldersLayout.get());

    setWidgetResizable(true);
    setWidget(foldersLayoutContainer.get());
    setMinimumWidth(200);
    setMaximumWidth(300);

    QTextEdit* textedit1 = new QTextEdit;
    textedit1->setFixedHeight(150);
    QTextEdit* textedit2 = new QTextEdit;
    textedit2->setFixedHeight(150);
    QTextEdit* textedit3 = new QTextEdit;
    textedit3->setFixedHeight(150);
    QTextEdit* textedit4 = new QTextEdit;
    textedit4->setFixedHeight(150);

    foldersLayout->setSizeConstraint(QLayout::SetMaximumSize);
    foldersLayout->addWidget(textedit1);
    foldersLayout->addWidget(textedit2);
    foldersLayout->addWidget(textedit3);
    foldersLayout->addWidget(textedit4);
    foldersLayout->setContentsMargins(InterfaceUtils::defaultMargins);
}
