#ifndef ConsoleIO_H
#define ConsoleIO_H

#include <QThread>
#include <QSocketNotifier>
#include <ncurses.h>

class ConsoleIO : public QObject
{
    Q_OBJECT
signals:
    void keyRead(int);
public slots:
    void write(char* output);
    void getWindow(WINDOW * nWin);
private slots:
    void readyRead();
public:
    ConsoleIO(QObject * parent = 0);
    WINDOW * win;
};

#endif  /* ConsoleIO_H */
