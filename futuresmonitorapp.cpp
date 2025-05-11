// futuresmonitorapp.cpp
#include "futuresmonitorapp.h"
#include "loginwindow.h"
#include "mainwindow.h"

FuturesMonitorApp::FuturesMonitorApp(int& argc, char** argv) : QApplication(argc, argv)
{
    // 设置应用程序样式
    setStyle("Fusion");

    // 创建登录窗口
    loginWindow = new LoginWindow();

    // 创建主窗口
    mainWindow = new MainWindow();

    // 连接登录成功信号
    connect(loginWindow, &LoginWindow::loginSuccess, this, &FuturesMonitorApp::showMainWindow);

    // 显示登录窗口
    loginWindow->show();
}

FuturesMonitorApp::~FuturesMonitorApp()
{
    delete mainWindow;
    delete loginWindow;
}

void FuturesMonitorApp::showMainWindow()
{
    loginWindow->hide();
    mainWindow->show();
}
