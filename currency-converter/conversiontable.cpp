#include "conversiontable.h"
#include "ui_conversiontable.h"

#include <QStandardItemModel>

conversionTable::conversionTable(double currencyValue, QWidget *parent)
    : QWidget(parent)
    , currencyValue(currencyValue)
    , ui(new Ui::conversionTable)
{
    ui->setupUi(this);
}

void conversionTable::updateExchange(const QString &baseCurrency, const QMap<QString, double> &rates)
{
    currentBaseCurrency = baseCurrency;
    currentRates = rates;

    // Set up the title label
    QLabel *title = findChild<QLabel *>("titleLabel");
    title->setText(tr("Conversion Table for %1").arg(baseCurrency));

    // Clear the table and populate it once you figure out how it works
    QTableView *tableView = findChild<QTableView *>("tableView");
    QStandardItemModel *model = new QStandardItemModel(this);

    model->setHorizontalHeaderLabels({tr("Currency"), tr("Exchange Rate"), tr("Converted Value")});

    for (auto it = rates.begin(); it != rates.end(); it++) {
        QString currencyName = it.key();
        double exchangeRate = it.value();
        double convertedValue = exchangeRate * currencyValue;

        QStandardItem *currencyItem = new QStandardItem(currencyName);
        QStandardItem *exchangeItem = new QStandardItem(QString::number(exchangeRate, 'f', 2));
        QStandardItem *convertedItem = new QStandardItem(QString::number(convertedValue, 'f', 2));

        QList<QStandardItem *> row = {currencyItem, exchangeItem, convertedItem};
        model->appendRow(row);
    }

    tableView->setModel(model);

    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setVisible(false);
    tableView->setSelectionMode(QAbstractItemView::NoSelection);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void conversionTable::updateCurrencyValue(double arg1)
{
    currencyValue = arg1;

    updateExchange(currentBaseCurrency, currentRates);
}

conversionTable::~conversionTable()
{
    delete ui;
}
