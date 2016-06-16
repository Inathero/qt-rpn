#ifndef QTRPNCALC_H
#define QTRPNCALC_H

#include <QObject>
#include <QCoreApplication>
#include <QHash>

#include "consoleio.h"

class QtRPNCalc : public QObject
{
    Q_OBJECT
public:
    explicit QtRPNCalc(QCoreApplication *parent = 0);

    QList<QString> ListOfCommands; //created in constructor

signals:

public slots:

    bool Calculate(QString input);

    QList<double> Operate(double x, double y, QString operation);

    void DisplayStack();

    bool PreparingForAdvancedCommands();

    void OnConsoleKeyPressed(int key);

    bool Operation(QString input);

    void Conveyor(QString conversion);

    void refreshScreen();

private:
    QList<double> Stack;
    bool bPrepareStore = false;
    bool bPrepareRecall = false;
    QHash<QString,double> RPNMemory;
    ConsoleIO * cIO;
    QString ConsoleReaderString;
};

#endif // QTRPNCALC_H
