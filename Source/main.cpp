#include <QCoreApplication>

#include "main.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    rpn = new QtRPNCalc();

    QString conversion;
    for (int i = 1; i < argc; i++)
        conversion.append(argv[i]).append(" ");
    conversion.chop(1);

    if(!conversion.isEmpty())
        rpn->Conveyor(conversion);
    else
        rpn->refreshScreen();

    int rc = a.exec();

    endwin();

    return rc;
}

