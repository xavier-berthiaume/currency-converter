#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "Checking current locales: " << QLocale::system().uiLanguages();

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "currency-converter_" + QLocale(locale).name();
        if (translator.load(":/translations/" + baseName + ".qm")) {
            qDebug() << "Found translation file at: " << ":/translations/" + baseName + ".qm";
            a.installTranslator(&translator);
            break;
        }
    }
    qDebug() << "Current translator set on program start: " << translator.filePath();

    MainWindow w;
    w.show();
    return a.exec();
}
