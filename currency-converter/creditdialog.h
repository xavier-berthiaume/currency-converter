#ifndef CREDITDIALOG_H
#define CREDITDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class creditDialog : public QDialog
{
    Q_OBJECT

    Ui::Dialog *ui;
public:
    explicit creditDialog(QWidget *parent = nullptr);
    ~creditDialog();
};

#endif // CREDITDIALOG_H
