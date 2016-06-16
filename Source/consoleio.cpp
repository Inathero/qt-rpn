#include "consoleio.h"
#include <QDebug>

void ConsoleIO::readyRead()
{
    // It's OK to call this with no data available to be read.
    int c;
    while ((c = getch()) != ERR)
    {
        if(c <= 255)
        {
            if(c == KEY_BACKSPACE || c == KEY_DC || c == 127)
            {
                int r;
                  getyx(win, r, c);
                  if(c > 2)
                  {
                  noecho();
                  nocbreak();
                  move(r, c-1);
                  delch();
                  cbreak();
                  }
                  refresh();

                  emit keyRead(127);
            }
            else
            {
            printw("%c", (char)c);
            emit keyRead(c);
            }
        }
    }
}

ConsoleIO::ConsoleIO(QObject *parent) : QObject(parent)
{
    connect(new QSocketNotifier(0, QSocketNotifier::Read, this),
            &QSocketNotifier::activated, this, &ConsoleIO::readyRead);
}

void ConsoleIO::write(char *output)
{
//    int * pOutput = (int*)output;
    for(output; output[0] != '\0'; output++)
        printw("%c", (char)output[0]);
}

void ConsoleIO::getWindow(WINDOW *nWin)
{
    win = nWin;
}
