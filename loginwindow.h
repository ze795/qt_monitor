#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include <QWidget>
#include <QDialog>
#include <QLineEdit>

#include "ui_loginwindow.h" // 自动生成的UI类头文件

class LoginWindow : public QDialog
{
    Q_OBJECT
public:
    explicit LoginWindow(QWidget* parent = nullptr);

signals:
    void loginSuccess();

private slots:
    void login();

    void on_loginButton_clicked();

private:
    Ui::LoginWindow ui; // UI类实例
};

#endif // LOGINWINDOW_H
