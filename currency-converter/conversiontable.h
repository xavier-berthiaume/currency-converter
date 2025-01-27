#ifndef CONVERSIONTABLE_H
#define CONVERSIONTABLE_H

#include <QWidget>

namespace Ui {
class conversionTable;
}

class conversionTable : public QWidget
{
    Q_OBJECT

    double currencyValue;
    QString currentBaseCurrency;
    QMap<QString, double> currentRates;

public:
    explicit conversionTable(double initialValue, QWidget *parent = nullptr);
    ~conversionTable();

public slots:
    void updateExchange(const QString &baseCurrency, const QMap<QString, double> &rates);
    void updateCurrencyValue(double arg1);


private:
    Ui::conversionTable *ui;
};

#endif // CONVERSIONTABLE_H
