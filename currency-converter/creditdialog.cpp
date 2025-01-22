#include "creditdialog.h"
#include "./ui_creditDialog.h"

creditDialog::creditDialog(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    setWindowTitle(tr("Credits"));
}

creditDialog::~creditDialog()
{
    delete ui;
}
