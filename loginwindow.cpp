// loginwindow.cpp
#include "loginwindow.h"
#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget* parent) : QDialog(parent)
{
    ui.setupUi(this); // 设置UI
//    connect(ui.loginButton, &QPushButton::clicked, this, &LoginWindow::login);
}

void LoginWindow::login()
{
    return;
}

void LoginWindow::on_loginButton_clicked()
{
    QString username = ui.usernameEdit->text();
    QString password = ui.passwordEdit->text();

    if (username == "admin" && password == "admin") {
        accept();
        emit loginSuccess();
    } else {
        QMessageBox::warning(this, "登录失败", "用户名或密码错误");
    }
}
