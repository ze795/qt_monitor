#ifndef FUTURESMONITORAPP_H
#define FUTURESMONITORAPP_H

#include <QApplication>

class LoginWindow;
class MainWindow;

class FuturesMonitorApp : public QApplication
{
    Q_OBJECT
public:
    FuturesMonitorApp(int& argc, char** argv);
    ~FuturesMonitorApp();

private slots:
    void showMainWindow();

private:
    LoginWindow* loginWindow;
    MainWindow* mainWindow;
};

#endif // FUTURESMONITORAPP_H
