#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"
// mainwindow.cpp
#include "mainwindow.h"
#include "datamanager.h"
#include "futuresitem.h"
#include <QLineEdit>
#include <QCheckBox>
#include <QTableWidget>


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    setWindowTitle("期货价格监测软件");
    setMinimumSize(1000, 600);

    dataManager = std::make_unique<DataManager>();
    dataManager->initFuturesItems();

    initializeTable();

    connect(dataManager.get(), &DataManager::dataUpdated, this, &MainWindow::updateTableData);
    dataManager->startMonitoring();
}

MainWindow::~MainWindow()
{
    dataManager->stopMonitoring();
}

void MainWindow::initializeTable()
{
    tableWidget = ui.tableWidget;
    tableWidget->setMinimumSize(1000, 600);
    tableWidget->setColumnCount(10);
    tableWidget->setHorizontalHeaderLabels({"序号", "品种", "当前价格", "监测价格", "价格监测开关",
                                          "ATR监测方向", "ATR监测开关", "形态监测方向", "形态监测开关",
                                          "当前状态"});

    // 设置表格属性
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    // 设置行数
    tableWidget->setRowCount(dataManager->getFuturesItems().size());

    // 初始化表格内容
    for (size_t i = 0; i < dataManager->getFuturesItems().size(); ++i) {
        const auto& item = dataManager->getFuturesItems()[i];

        // 序号
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(item->id));
        tableWidget->setItem(i, 0, idItem);

        // 品种
        QTableWidgetItem* nameItem = new QTableWidgetItem(item->name);
        tableWidget->setItem(i, 1, nameItem);

        // 当前价格
        QTableWidgetItem* priceItem = new QTableWidgetItem(QString::number(item->currentPrice, 'f', 2));
        tableWidget->setItem(i, 2, priceItem);

        // 监测价格
        QLineEdit* monitorPriceEdit = new QLineEdit(QString::number(item->monitorPrice, 'f', 2));
        tableWidget->setCellWidget(i, 3, monitorPriceEdit);
        connect(monitorPriceEdit, &QLineEdit::textChanged, [this, i](const QString& text) {
            bool ok;
            double value = text.toDouble(&ok);
            if (ok) {
                dataManager->getFuturesItems()[i]->monitorPrice = value;
            }
        });

        // 价格监测开关
        QCheckBox* priceMonitorCheck = new QCheckBox();
        priceMonitorCheck->setChecked(item->priceMonitorEnabled);
        tableWidget->setCellWidget(i, 4, priceMonitorCheck);
        connect(priceMonitorCheck, &QCheckBox::stateChanged, [this, i](int state) {
            dataManager->getFuturesItems()[i]->priceMonitorEnabled = (state == Qt::Checked);
        });

        // ATR监测方向
        QComboBox* atrDirectionCombo = new QComboBox();
        atrDirectionCombo->addItems({"多", "空"});
        atrDirectionCombo->setCurrentText(item->atrDirection);
        tableWidget->setCellWidget(i, 5, atrDirectionCombo);
        connect(atrDirectionCombo, &QComboBox::currentTextChanged, [this, i](const QString& text) {
            dataManager->getFuturesItems()[i]->atrDirection = text;
        });

        // ATR监测开关
        QCheckBox* atrMonitorCheck = new QCheckBox();
        atrMonitorCheck->setChecked(item->atrMonitorEnabled);
        tableWidget->setCellWidget(i, 6, atrMonitorCheck);
        connect(atrMonitorCheck, &QCheckBox::stateChanged, [this, i](int state) {
            dataManager->getFuturesItems()[i]->atrMonitorEnabled = (state == Qt::Checked);
        });

        // 形态监测方向
        QComboBox* patternDirectionCombo = new QComboBox();
        patternDirectionCombo->addItems({"多", "空"});
        patternDirectionCombo->setCurrentText(item->patternDirection);
        tableWidget->setCellWidget(i, 7, patternDirectionCombo);
        connect(patternDirectionCombo, &QComboBox::currentTextChanged, [this, i](const QString& text) {
            dataManager->getFuturesItems()[i]->patternDirection = text;
        });

        // 形态监测开关
        QCheckBox* patternMonitorCheck = new QCheckBox();
        patternMonitorCheck->setChecked(item->patternMonitorEnabled);
        tableWidget->setCellWidget(i, 8, patternMonitorCheck);
        connect(patternMonitorCheck, &QCheckBox::stateChanged, [this, i](int state) {
            dataManager->getFuturesItems()[i]->patternMonitorEnabled = (state == Qt::Checked);
        });

        // 当前状态
        QTableWidgetItem* statusItem = new QTableWidgetItem(item->status);
        statusItem->setForeground(item->statusColor);
        tableWidget->setItem(i, 9, statusItem);
    }
}

void MainWindow::updateTableData()
{
    for (size_t i = 0; i < dataManager->getFuturesItems().size(); ++i) {
        const auto& item = dataManager->getFuturesItems()[i];

        // 更新当前价格
        QTableWidgetItem* priceItem = tableWidget->item(i, 2);
        if (priceItem) {
            priceItem->setText(QString::number(item->currentPrice, 'f', 2));
        }

        // 更新当前状态
        QTableWidgetItem* statusItem = tableWidget->item(i, 9);
        if (statusItem) {
            statusItem->setText(item->status);
            statusItem->setForeground(item->statusColor);
        }
    }

    ui.tableWidget->viewport()->update();
}
