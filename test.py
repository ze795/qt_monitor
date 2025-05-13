import sys
import time
import threading
import random
import datetime
from PyQt5.QtWidgets import (QApplication, QMainWindow, QTableWidget, QTableWidgetItem, QPushButton,
                            QVBoxLayout, QHBoxLayout, QWidget, QLabel, QLineEdit, QComboBox,
                            QMessageBox, QCheckBox, QDialog, QHeaderView, QAbstractItemView,
                            QToolBar, QAction)
from PyQt5.QtCore import Qt, QTimer, pyqtSignal, QObject
from PyQt5.QtGui import QColor

from data import ATRCalculator, getRealTimeP
from typing import List, Dict, Tuple, Optional

# Futures data structure
class FuturesItem:
    def __init__(self, id, name, symbol="default"):
        self.id = id
        self.name = name
        self.symbol = symbol
        self.currentPrice = 0.0
        self.monitorPrice = 0.0
        self.priceMonitorEnabled = False
        self.atrDirection = "Long"  # "Long" or "Short"
        self.atrMonitorEnabled = False
        self.patternDirection = "Long"  # "Long" or "Short"
        self.patternMonitorEnabled = False
        self.status = "Normal"
        self.statusColor = Qt.black

# Pattern Recognition Class (To be implemented by user)
class PatternRecognizer:
    @staticmethod
    def recognizePattern(symbol, direction):
        # Implement pattern recognition logic here
        # Return random True or False for demonstration
        return random.choice([True, False])

# Data Manager Class
class DataManager(QObject):
    dataUpdated = pyqtSignal()

    def __init__(self):
        super().__init__()
        self.futuresItems: List[FuturesItem] = []
        self.stopRequested = False
        self.monitoringThread = None

    def initFuturesItems(self):
        self.futuresItems = []
        self.futuresItems.append(FuturesItem(0, "PVC Continuous", "V0"))
        self.futuresItems.append(FuturesItem(1, "zonglv Continuous", "P0"))
        self.futuresItems.append(FuturesItem(2, "Soybean No.2 Continuous", "B0"))
        self.futuresItems.append(FuturesItem(3, "Soybean Meal Continuous", "M0"))
        self.futuresItems.append(FuturesItem(4, "tiekuang Continuous", "I0"))
        self.futuresItems.append(FuturesItem(5, "Plastic Continuous", "L0"))
        self.futuresItems.append(FuturesItem(6, "jubingxi Continuous", "PP0"))
        self.futuresItems.append(FuturesItem(7, "Soybean Oil Continuous", "Y0"))
        self.futuresItems.append(FuturesItem(8, "yumi Continuous", "C0"))
        self.futuresItems.append(FuturesItem(9, "Soybean No.1 Continuous", "A0"))
        self.futuresItems.append(FuturesItem(10, "benyixi Continuous", "EB0"))
        self.futuresItems.append(FuturesItem(11, "PTA Continuous", "TA0"))
        self.futuresItems.append(FuturesItem(12, "caiyou Continuous", "OI0"))
        self.futuresItems.append(FuturesItem(13, "caipo Continuous", "RM0"))
        self.futuresItems.append(FuturesItem(14, "Sugar Continuous", "SR0"))
        self.futuresItems.append(FuturesItem(15, "mianhua Continuous", "CF0"))
        self.futuresItems.append(FuturesItem(16, "jiachun Continuous", "MA0"))
        self.futuresItems.append(FuturesItem(17, "Glass Continuous", "FG0"))
        self.futuresItems.append(FuturesItem(18, "hongzao Continuous", "CJ0"))
        self.futuresItems.append(FuturesItem(19, "chunjian Continuous", "SA0"))
        self.futuresItems.append(FuturesItem(20, "luowen Continuous", "RB0"))
        self.futuresItems.append(FuturesItem(21, "zhijiang Continuous", "SP0"))

    def getFuturesItems(self):
        return self.futuresItems

    def startMonitoring(self):
        if self.monitoringThread and self.monitoringThread.is_alive():
            return

        self.stopRequested = False
        self.monitoringThread = threading.Thread(target=self.monitoringLoop)
        self.monitoringThread.daemon = True
        self.monitoringThread.start()

    def stopMonitoring(self):
        self.stopRequested = True
        if self.monitoringThread and self.monitoringThread.is_alive():
            self.monitoringThread.join(timeout=1.0)

    def monitoringLoop(self):
        while not self.stopRequested:
            for item in self.futuresItems:
                # Update current price and datatable
                item.currentPrice = getRealTimeP(item.symbol)

                # Price monitoring
                if item.priceMonitorEnabled and abs(item.currentPrice - item.monitorPrice) <= 2.0:
                    item.status = "Price Alert"
                    item.statusColor = Qt.red
                # ATR monitoring
                elif item.atrMonitorEnabled:
                    # atr = ATRCalculator.calculateATR(item.name)
                    # Judge based on ATR value and direction
                    atrCondition = ATRCalculator.atr_cond(item.symbol, item.atrDirection)
                    
                    if atrCondition:
                        item.status = "ATR Alert"
                        item.statusColor = Qt.blue
                # Pattern monitoring
                elif item.patternMonitorEnabled:
                    if PatternRecognizer.recognizePattern(item.name, item.patternDirection):
                        item.status = "Pattern Alert"
                        item.statusColor = Qt.green
                else:
                    item.status = "Normal"
                    item.statusColor = Qt.black

            self.dataUpdated.emit()

            # Update interval
            time.sleep(20)

# Login Window Class
class LoginWindow(QDialog):
    loginSuccess = pyqtSignal()

    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Login")
        self.setFixedSize(300, 200)

        # Create layout
        mainLayout = QVBoxLayout(self)

        # Add username and password input fields
        usernameLabel = QLabel("Username:", self)
        self.usernameEdit = QLineEdit(self)

        passwordLabel = QLabel("Password:", self)
        self.passwordEdit = QLineEdit(self)
        self.passwordEdit.setEchoMode(QLineEdit.Password)

        # Add login button
        loginButton = QPushButton("Login", self)
        loginButton.clicked.connect(self.login)

        # Add widgets to layout
        mainLayout.addWidget(usernameLabel)
        mainLayout.addWidget(self.usernameEdit)
        mainLayout.addWidget(passwordLabel)
        mainLayout.addWidget(self.passwordEdit)
        mainLayout.addStretch()
        mainLayout.addWidget(loginButton)

        # Set default values for testing
        self.usernameEdit.setText("admin")
        self.passwordEdit.setText("admin")

    def login(self):
        username = self.usernameEdit.text()
        password = self.passwordEdit.text()

        # Simple authentication (should connect to database or other authentication method in actual application)
        if username == "admin" and password == "admin":
            self.accept()
            self.loginSuccess.emit()
        else:
            QMessageBox.warning(self, "Login Failed", "Invalid username or password")

# Main Window Class
class MainWindow(QMainWindow):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("Futures Price Monitoring Software")
        self.resize(1000, 600)

        # Initialize data manager
        self.dataManager = DataManager()
        self.dataManager.initFuturesItems()

        self.columnsHidden = False
        
        # Create central widget and layout
        centralWidget = QWidget(self)
        self.setCentralWidget(centralWidget)

        mainLayout = QVBoxLayout(centralWidget)

        self.createToolBar()
        
        # Create table
        self.createTable()

        # Add to layout
        mainLayout.addWidget(self.tableWidget)
        self.tableWidget.setSelectionMode(QTableWidget.NoSelection)

        # Connect data update signal
        self.dataManager.dataUpdated.connect(self.updateTableData)

        # Start monitoring thread
        self.dataManager.startMonitoring()
     
    def createToolBar(self):
        toolBar = QToolBar("Tool Bar", self)
        self.addToolBar(toolBar)

        toggleColumnsAction = QAction("Show/Hide Columns", self)
        toggleColumnsAction.setStatusTip("Toggle display of columns (Only show first 3 columns)")
        toggleColumnsAction.triggered.connect(self.toggleColumns)
        toolBar.addAction(toggleColumnsAction)
    
    def toggleColumns(self):
        if self.columnsHidden:
            for col in range(10):
                self.tableWidget.setColumnHidden(col, False)
            self.columnsHidden = False
            # self.setMinimumSize(1000, 600)
        else:
            for col in range(3, 10):
                self.tableWidget.setColumnHidden(col, True)
            self.columnsHidden = True
            self.resize(200, 600)
    
    def createTable(self):
        self.tableWidget = QTableWidget(self)
        self.tableWidget.setColumnCount(9)
        # self.tableWidget.setHorizontalHeaderLabels(["ID", "Symbol", "Current Price", "Monitor Price", "Price Monitor",
        #                                       "ATR Direction", "ATR Monitor", "Pattern Direction", "Pattern Monitor",
        #                                       "Status"])
        self.tableWidget.setHorizontalHeaderLabels(["Status", "Symbol", "Current P", "Monitor P", "P Monitor",
                                              "ATR Dir", "ATR Monitor", "Pattern Dir", "Pattern Monitor"])

        # Set table properties
        header = self.tableWidget.horizontalHeader()
        header.setSectionResizeMode(QHeaderView.Interactive)
        self.tableWidget.setEditTriggers(QAbstractItemView.NoEditTriggers)
        self.tableWidget.setSelectionBehavior(QAbstractItemView.SelectRows)

        # Set row count
        self.tableWidget.setRowCount(len(self.dataManager.getFuturesItems()))

        # Initialize table content
        for i, item in enumerate(self.dataManager.getFuturesItems()):
            # Status
            statusItem = QTableWidgetItem(item.status)
            statusItem.setForeground(item.statusColor)
            self.tableWidget.setItem(i, 0, statusItem)

            # Symbol
            nameItem = QTableWidgetItem(item.name)
            self.tableWidget.setItem(i, 1, nameItem)

            # Current Price
            priceItem = QTableWidgetItem("{0:.2f}".format(item.currentPrice))
            self.tableWidget.setItem(i, 2, priceItem)

            # Monitor Price
            monitorPriceEdit = QLineEdit("{0:.2f}".format(item.monitorPrice))
            self.tableWidget.setCellWidget(i, 3, monitorPriceEdit)
            monitorPriceEdit.textChanged.connect(lambda text, row=i: self.onMonitorPriceChanged(text, row))

            # Price Monitor Switch
            priceMonitorCheck = QCheckBox()
            priceMonitorCheck.setChecked(item.priceMonitorEnabled)
            self.tableWidget.setCellWidget(i, 4, priceMonitorCheck)
            priceMonitorCheck.stateChanged.connect(lambda state, row=i: self.onPriceMonitorChanged(state, row))

            # ATR Direction
            atrDirectionCombo = QComboBox()
            atrDirectionCombo.addItems(["Long", "Short"])
            atrDirectionCombo.setCurrentText(item.atrDirection)
            self.tableWidget.setCellWidget(i, 5, atrDirectionCombo)
            atrDirectionCombo.currentTextChanged.connect(lambda text, row=i: self.onAtrDirectionChanged(text, row))

            # ATR Monitor Switch
            atrMonitorCheck = QCheckBox()
            atrMonitorCheck.setChecked(item.atrMonitorEnabled)
            self.tableWidget.setCellWidget(i, 6, atrMonitorCheck)
            atrMonitorCheck.stateChanged.connect(lambda state, row=i: self.onAtrMonitorChanged(state, row))

            # Pattern Direction
            patternDirectionCombo = QComboBox()
            patternDirectionCombo.addItems(["Long", "Short"])
            patternDirectionCombo.setCurrentText(item.patternDirection)
            self.tableWidget.setCellWidget(i, 7, patternDirectionCombo)
            patternDirectionCombo.currentTextChanged.connect(lambda text, row=i: self.onPatternDirectionChanged(text, row))

            # Pattern Monitor Switch
            patternMonitorCheck = QCheckBox()
            patternMonitorCheck.setChecked(item.patternMonitorEnabled)
            self.tableWidget.setCellWidget(i, 8, patternMonitorCheck)
            patternMonitorCheck.stateChanged.connect(lambda state, row=i: self.onPatternMonitorChanged(state, row))

            # ID
            # idItem = QTableWidgetItem(str(item.id))
            # self.tableWidget.setItem(i, 9, idItem)


    def onMonitorPriceChanged(self, text, row):
        try:
            value = float(text)
            self.dataManager.getFuturesItems()[row].monitorPrice = value
        except ValueError:
            pass

    def onPriceMonitorChanged(self, state, row):
        self.dataManager.getFuturesItems()[row].priceMonitorEnabled = (state == Qt.Checked)

    def onAtrDirectionChanged(self, text, row):
        self.dataManager.getFuturesItems()[row].atrDirection = text

    def onAtrMonitorChanged(self, state, row):
        self.dataManager.getFuturesItems()[row].atrMonitorEnabled = (state == Qt.Checked)

    def onPatternDirectionChanged(self, text, row):
        self.dataManager.getFuturesItems()[row].patternDirection = text

    def onPatternMonitorChanged(self, state, row):
        self.dataManager.getFuturesItems()[row].patternMonitorEnabled = (state == Qt.Checked)

    def updateTableData(self):
        for i, item in enumerate(self.dataManager.getFuturesItems()):
            # Update current price
            priceItem = self.tableWidget.item(i, 2)
            if priceItem:
                priceItem.setText("{0:.2f}".format(item.currentPrice))

            # Update current status
            statusItem = self.tableWidget.item(i, 0)
            if statusItem:
                statusItem.setText(item.status)
                statusItem.setForeground(item.statusColor)

# Main function
if __name__ == "__main__":
    app = QApplication(sys.argv)

    # Show login window
    loginWindow = LoginWindow()
    if loginWindow.exec_() != QDialog.Accepted:
        sys.exit()

    # Show main window
    mainWindow = MainWindow()
    mainWindow.show()

    sys.exit(app.exec_())
