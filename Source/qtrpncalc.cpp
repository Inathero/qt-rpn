#include "qtrpncalc.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <QDebug>
#include <iostream>
using namespace std;

QtRPNCalc::QtRPNCalc(QCoreApplication *parent) : QObject(parent)
{
    // ORDER IS IMPORTANT
    // ONLY ADD NEW COMMANDS AT THE END
    ListOfCommands.append("+"); // 0
    ListOfCommands.append("-"); // 1
    ListOfCommands.append("/"); // 2
    ListOfCommands.append("*"); // 3
    ListOfCommands.append(""); // 4 /// Caccel this
    ListOfCommands.append("clear"); // 5
    ListOfCommands.append("swap"); // 6
    ListOfCommands.append("sw"); // 7
    ListOfCommands.append("sq"); // 8
    ListOfCommands.append("sqrt"); // 9
    ListOfCommands.append("^"); // 10
    ListOfCommands.append("neg"); // 11
    ListOfCommands.append("chs"); // 12
    ListOfCommands.append("inv"); // 13
    ListOfCommands.append("drop"); // 14

    //trig functions
    ListOfCommands.append("sin");
    ListOfCommands.append("cos");
    ListOfCommands.append("tan");
    ListOfCommands.append("asin");
    ListOfCommands.append("acos");
    ListOfCommands.append("atan");


    ListOfCommands.append("abs");
    ListOfCommands.append("e");
    ListOfCommands.append("exp");
    ListOfCommands.append("ln");
    ListOfCommands.append("log");

    // Advanced Commands
    ListOfCommands.append("sto");
    ListOfCommands.append("rcl");



    cIO = new ConsoleIO(this);
    cIO->getWindow(initscr());
    start_color();
    cbreak(); // all input is available immediately
    noecho(); // no echo
    keypad(cIO->win, true); // special keys are interpreted and returned as single int from getch()
    nodelay(cIO->win, true); // getch() is a non-blocking call
    scrollok(cIO->win, true);
    connect (cIO, &ConsoleIO::keyRead, this, &QtRPNCalc::OnConsoleKeyPressed);

    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
}

bool QtRPNCalc::Calculate(QString input)
{
    if(!ListOfCommands.contains(input) && !PreparingForAdvancedCommands()) //Either number or undetermined function
    {
        bool isDouble;
        double dNumber = input.toDouble(&isDouble);
        if(isDouble)
            Stack.push_back(dNumber);
        else
            return false;

    }
    else // is a function
    {
        double x = Stack.isEmpty() ? 0 : Stack.last();

        if(!Stack.isEmpty())
            Stack.pop_back();

        double y = Stack.isEmpty() ? 0 : Stack.last();
        if(!Stack.isEmpty())
            Stack.pop_back();

        QList<double> results = Operate(x, y, input);
        foreach(double result, results)
            Stack.push_back(result);

    }
    return true;
}

QList<double> QtRPNCalc::Operate(double x, double y, QString operation)
{
    QList<double> results;
    switch(ListOfCommands.indexOf(operation))
    {
    case 0: // +
        results.append(x + y);
        break;
    case 1: // -
        results.append(y - x);
        break;
    case 2: // /
        results.append(y / x);
        break;
    case 3: // *
        results.append(x * y);
        break;
    case 4: // return
        results.append(y);
        results.append(x);
//        results.append(x);
        break;
    case 5: // clear
        Stack.clear();
        clear();
//        refreshScreen();
        break;
    case 6: // swap
    case 7: // sw
        results.append(x);
        results.append(y);
        break;
    case 8: // sq (square)
        results.append(y);
        x = pow(x,2);
        results.append(x);
        break;
    case 9: // sqrt (square root)
        results.append(y);
        x = pow(x,0.5);
        results.append(x);
        break;
    case 10: // ^ (power)
        x = pow(y, x);
        results.append(x);
        break;
    case 11: // neg (turn x negative)
    case 12: // chs (turn x negative / change sign)
        results.append(y);
        x = x * -1;
        results.append(x);
        break;
    case 13: // inv (invert)
        results.append(y);
        x = 1 / x;
        results.append(x);
        break;
    case 14: // drop last number in stack
        results.append(y);
        break;

    /// Trig Section
    ///
    case 15: //
        results.append(y);
        x = sin(x);
        results.append(x);
        break;
    case 16: //
        results.append(y);
        x = cos(x);
        results.append(x);
        break;
    case 17: //
        results.append(y);
        x = tan(x);
        results.append(x);
        break;
    case 18: //
        results.append(y);
        x = asin(x);
        results.append(x);
        break;
    case 19: //
        results.append(y);
        x = acos(x);
        results.append(x);
        break;
    case 20: //
        results.append(y);
        x = atan(x);
        results.append(x);
        break;
    /// End Trig Section

    case 21: // absolute
        results.append(y);
        x = abs(x);
        results.append(x);
        break;
    case 22: // e
        results.append(y);
        results.append(x);
        results.append(exp(1));
        break;
    case 23: // exp (e ^ x )
        results.append(y);
        x = exp(x);
        results.append(x);
        break;
    case 24: // ln
        results.append(y);
        x = log(x);
        results.append(x);
        break;
    case 25: // log
        results.append(y);
        x = log10(x);
        results.append(x);
        break;

        /// Start Advanced
    case 26: // store into memory
        results.append(y);
        results.append(x);
        bPrepareStore = true;
        break;

    case 27: // Recall from memory
        results.append(y);
        results.append(x);
        bPrepareRecall = true;
        break;

    case -1: // Advanced commands
    {
        results.append(y);
        results.append(x);
        if(bPrepareStore)
        {
            bPrepareStore = false;
            RPNMemory.insert(operation, x);
        }
        if(bPrepareRecall)
        {
           bPrepareRecall = false;
            results.append(RPNMemory.value(operation));
        }
    }
        break;
    }
    return results;
}

void QtRPNCalc::DisplayStack()
{
    int sizeOfStack = Stack.size() - 1;
    char buf[256];

    for(int i = 0; i < Stack.size(); i++)
    {
        #ifdef _WIN32
        qDebug() << sizeOfStack << ": " << Stack.at(i) << endl;
#endif
#ifdef __linux__
        if(sizeOfStack == 1)
        {
            attron(COLOR_PAIR(2));
            printw("%d", sizeOfStack);
            attroff(COLOR_PAIR(2));
            printw(" : %g",  Stack.at(i));
        }
        else if(sizeOfStack == 0)
        {
            attron(COLOR_PAIR(1));
            printw("%d", sizeOfStack);
            attroff(COLOR_PAIR(1));
            printw(" : %g",  Stack.at(i));
        }
        else
            printw("%d : %g", sizeOfStack, Stack.at(i));
        printw("\n");
        refresh();
#endif
        sizeOfStack --;
    }
    printw("\n");
}

bool QtRPNCalc::PreparingForAdvancedCommands()
{
    return bPrepareStore ? true : bPrepareRecall ? true : false;
}

void QtRPNCalc::OnConsoleKeyPressed(int key)
{
//    qDebug() << "QtRPNCalc::OnConsoleKeyPressed:"<< key;
    if(key == 127) // Delete key
    {
        ConsoleReaderString.chop(1);
        return;
    }
    else
        ConsoleReaderString.append((char)key);

    if(ListOfCommands.contains(ConsoleReaderString) || ConsoleReaderString.contains(' ') || key == 10)
    {
        printw("\n");
        Conveyor(ConsoleReaderString);
        ConsoleReaderString.clear();
        return;
    }
    // i.e. 5+ or 123-
    if(!ListOfCommands.contains(ConsoleReaderString) && ListOfCommands.contains(QString((char)key)))
    {
        // This was a terrible mistake...
        if((char)key == 'e')
        {
            //could be 'leave', you never know ..
            Operation(ConsoleReaderString);
            return;
        }
        ConsoleReaderString.remove((char)key);
        printw("\n");
        Conveyor(ConsoleReaderString);
        ConsoleReaderString.clear();
        printw("\n");
        Conveyor(QString((char)key));

        return;
    }

    if(Operation(ConsoleReaderString))
    {
        ConsoleReaderString.clear();
        refreshScreen();
    }
}

bool QtRPNCalc::Operation(QString input)
{
    if (input.contains("exit", Qt::CaseInsensitive) ||
            input.contains("quit", Qt::CaseInsensitive) ||
            input.contains("leave", Qt::CaseInsensitive) ||
            input.contains(":q", Qt::CaseInsensitive))
        {
            QCoreApplication::exit(0);
            return true;
        }
        if (input.contains("help", Qt::CaseInsensitive) ||
                input.contains("?", Qt::CaseInsensitive) ||
                input.contains(":h", Qt::CaseInsensitive))
        {
            printw("\n\n");
            printw("The following operations and constants are recognized: \n\n");
            int formationCounter = 0;
            foreach (QString command, ListOfCommands)
            {
                formationCounter++;
                if(command.isEmpty())
                    command = "space/return";

                printw("%s \t", command.toLocal8Bit().data());
                if(formationCounter > 4)
                {
                    formationCounter = 0;
                    printw("\n");
//                    qDebug() << "\n";
                }
            }
//            qDebug() << "\n\n";
            printw("\n\n");
            return true;
        }
        return false;
}

void QtRPNCalc::Conveyor(QString conversion)
{

//    QByteArray item = events.read(125);
//    qDebug() << (item.at(item.length()-1)) << endl;
        if (!Operation(conversion))
        {
            QStringList conveyor = conversion.split(' ');

            foreach (QString item, conveyor)
            {
                if (!Calculate(item))
                {

                    attron(COLOR_PAIR(3));
//                    item.chop(1);
                    printw("ERROR: %s is not an acceptable input\n", item.toLocal8Bit().data());
//                    qDebug() << "ERROR: " << item.toLocal8Bit().data() << " is not an acceptable input\n";
                    attroff(COLOR_PAIR(3));
                }
            }
        }
        refreshScreen();
}


void QtRPNCalc::refreshScreen()
{
    DisplayStack();
        attron(COLOR_PAIR(1));
    printw("> ");
        attroff(COLOR_PAIR(1));
    refresh();
}
