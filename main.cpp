#include "mainwindow.h"

#include <QApplication>

#include "futuresmonitorapp.h"

int main(int argc, char** argv)
{
    FuturesMonitorApp app(argc, argv);
    return app.exec();
}
