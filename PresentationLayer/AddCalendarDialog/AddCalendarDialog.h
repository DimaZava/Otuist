#ifndef ADDCALENDARDIALOG_H
#define ADDCALENDARDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QListWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>
#include <QToolBar>
#include <QVBoxLayout>
#include <set>

struct AddCalendarDTO
{
    std::string name;
    std::set<std::string> categories;
};

class AddCalendarDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AddCalendarDialog(QWidget* parent = nullptr);
    AddCalendarDialog(const AddCalendarDialog& dialog) = delete;
    AddCalendarDialog(AddCalendarDialog&& dialog) = delete;
    ~AddCalendarDialog() override;

    AddCalendarDialog& operator=(const AddCalendarDialog& dialog) = delete;
    AddCalendarDialog& operator=(AddCalendarDialog&& dialog) = delete;

    AddCalendarDTO getReturnValue();

private:
    void setupInitialState();

    std::unique_ptr<QFormLayout> formLayout;
    std::unique_ptr<QTextEdit> nameTextEdit;
    std::unique_ptr<QWidget> categoriesContainerWidget;
    std::unique_ptr<QVBoxLayout> categoriesLayout;
    std::unique_ptr<QScrollArea> categoriesListScrollArea;
    std::unique_ptr<QWidget> categoriesListContainerWidget;
    std::unique_ptr<QVBoxLayout> categoriesList;
    std::unique_ptr<QToolBar> categoriesToolbar;
    std::unique_ptr<QAction> addAction;
    std::unique_ptr<QAction> removeAction;
    std::unique_ptr<QPushButton> saveButton;

private slots:
    void saveButtonDidClick(bool checked);
    void addCategoryActionTriggered();
    void removeCategoryActionTriggered();
};

#endif // ADDCALENDARDIALOG_H
