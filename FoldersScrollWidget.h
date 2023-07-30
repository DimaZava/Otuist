#ifndef FOLDERSSCROLLWIDGET_H
#define FOLDERSSCROLLWIDGET_H

#include <QScrollArea>
#include <QVBoxLayout>

class FoldersScrollWidget : public QScrollArea
{
    Q_OBJECT
public:
    FoldersScrollWidget(QWidget* parent = nullptr);
    ~FoldersScrollWidget();

private:
    std::unique_ptr<QWidget> foldersLayoutContainer;
    std::unique_ptr<QVBoxLayout> foldersLayout;

    void configureLayout();
};

#endif // FOLDERSSCROLLWIDGET_H
