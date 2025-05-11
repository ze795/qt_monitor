#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QtCore>
#include <memory>
#include <vector>
#include <atomic>
#include "futuresitem.h"

class FuturesItem;

class DataManager : public QObject
{
    Q_OBJECT
public:
    explicit DataManager(QObject* parent = nullptr);

    void initFuturesItems();
    std::vector<std::unique_ptr<FuturesItem>>& getFuturesItems();
    void startMonitoring();
    void stopMonitoring();

signals:
    void dataUpdated();

private:
    std::vector<std::unique_ptr<FuturesItem>> futuresItems;
    std::unique_ptr<std::thread> monitoringThread;
    std::mutex dataMutex;
    std::atomic<bool> stopRequested;

    double getRealTimePrice(const QString& symbol);
    void monitoringLoop();
};

#endif // DATAMANAGER_H
