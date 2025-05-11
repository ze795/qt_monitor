#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <memory>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


#include "ui_mainwindow.h"
#include "datamanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    void initializeTable();
    ~MainWindow();
    QTableWidget * tableWidget;
private slots:
    void updateTableData();

private:
    Ui::MainWindow ui;
    std::unique_ptr<DataManager> dataManager;

};

#endif // MAINWINDOW_H
