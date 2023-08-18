#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H

#include <QDateTimeEdit>
#include <QDialog>
#include <QFormLayout>
#include <QPushButton>
#include <QTextEdit>

struct AddEventDialogDTO
{
    std::string name;
    std::string calendarName;
    std::string category;
    std::chrono::time_point<std::chrono::system_clock> beginDateTime;
    std::optional<std::chrono::time_point<std::chrono::system_clock>> endDateTime;
    std::optional<std::string> description;
};

class AddEventDialog : public QDialog
{
    Q_OBJECT
public:
    AddEventDialog(
        std::optional<std::chrono::time_point<std::chrono::system_clock>> beginDateTime,
        std::optional<std::chrono::time_point<std::chrono::system_clock>> endDateTime,
        QWidget* parent = nullptr);

    AddEventDialog(const AddEventDialog& dialog) = delete;
    AddEventDialog(AddEventDialog&& dialog) = delete;

    ~AddEventDialog() override;

    AddEventDialog& operator=(const AddEventDialog& dialog) = delete;
    AddEventDialog& operator=(AddEventDialog&& dialog) = delete;

    AddEventDialogDTO getReturnValue();

private:
    void setupInitialState(
        std::optional<std::chrono::time_point<std::chrono::system_clock>> beginDateTime,
        std::optional<std::chrono::time_point<std::chrono::system_clock>> endDateTime);

    std::unique_ptr<QFormLayout> formLayout;
    std::unique_ptr<QTextEdit> nameTextEdit;
    std::unique_ptr<QTextEdit> calendarNameTextEdit;
    std::unique_ptr<QTextEdit> categoryNameTextEdit;
    std::unique_ptr<QDateTimeEdit> beginDateTimePicker;
    std::unique_ptr<QDateTimeEdit> endDateTimePicker;
    std::unique_ptr<QTextEdit> descriptionTextEdit;
    std::unique_ptr<QPushButton> saveButton;

private slots:
    void saveButtonDidClick(bool checked);
};

#endif // ADDEVENTDIALOG_H
