#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

    const QString &currentLang;

    void populateLanguages();

public:
    explicit SettingsDialog(const QString &currentLang, QWidget *parent = nullptr);
    ~SettingsDialog();

    QString getSelectedLanguage();

private slots:
    void onCloseButtonClicked();

    void onSaveButtonClicked();

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
