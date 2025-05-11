// datamanager.cpp
#include "datamanager.h"
#include "futuresitem.h"
#include "atrcalculator.h"
#include "patternrecognizer.h"
#include <random>

DataManager::DataManager(QObject* parent) : QObject(parent), stopRequested(false)
{
}

void DataManager::initFuturesItems()
{
    futuresItems.clear();
    futuresItems.push_back(std::make_unique<FuturesItem>(0, "PVC连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(1, "棕榈油连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(2, "豆二连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(3, "豆粕连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(4, "铁矿石连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(5, "塑料连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(6, "聚丙烯连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(7, "豆油连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(8, "玉米连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(9, "豆一连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(10, "苯乙烯连续ac"));
    futuresItems.push_back(std::make_unique<FuturesItem>(11, "PTA连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(12, "菜油连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(13, "菜粕连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(14, "白糖连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(15, "棉花连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(16, "甲醇连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(17, "玻璃连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(18, "红枣连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(19, "纯碱连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(20, "螺纹钢连续"));
    futuresItems.push_back(std::make_unique<FuturesItem>(21, "纸浆连续"));
}

std::vector<std::unique_ptr<FuturesItem>>& DataManager::getFuturesItems()
{
    return futuresItems;
}

void DataManager::startMonitoring()
{
    if (monitoringThread && stopRequested) {
        return;
    }

    stopRequested = false;
    monitoringThread = std::make_unique<std::thread>(&DataManager::monitoringLoop, this);
}

void DataManager::stopMonitoring()
{
    stopRequested = true;
    if (monitoringThread && monitoringThread->joinable()) {
        monitoringThread->join();
    }
}

double DataManager::getRealTimePrice(const QString& symbol)
{
    // 实际应用中应使用akshare获取真实价格
    // 这里使用随机数模拟价格波动
    static bool initialized = false;
    if (!initialized) {
        qsrand(QDateTime::currentDateTime().toTime_t());
        initialized = true;
    }

    // 模拟价格在3000-8000之间
    return 3440;
}

void DataManager::monitoringLoop()
{
    while (!stopRequested) {
        {
            std::lock_guard<std::mutex> lock(dataMutex);
            for (auto& item : futuresItems) {
                // 更新当前价格
                item->currentPrice = getRealTimePrice(item->name);

                // 价格监测
                if (item->priceMonitorEnabled && qAbs(item->currentPrice - item->monitorPrice) <= 2.0) {
                    item->status = "价格预警";
                    item->statusColor = Qt::red;
                }
                // ATR监测
                else if (item->atrMonitorEnabled) {
                    double atr = ATRCalculator::calculateATR(item->name);
                    // 这里根据ATR值和方向进行判断
                    // 示例中简单判断
                    bool atrCondition = (item->atrDirection == "多" && atr > 20.0) ||
                                       (item->atrDirection == "空" && atr < 10.0);

                    if (atrCondition) {
                        item->status = "ATR预警";
                        item->statusColor = Qt::blue;
                    }
                }
                // 形态监测
                else if (item->patternMonitorEnabled) {
                    if (PatternRecognizer::recognizePattern(item->name, item->patternDirection)) {
                        item->status = "形态预警";
                        item->statusColor = Qt::green;
                    }
                }
                else {
                    item->status = "正常";
                    item->statusColor = Qt::black;
                }
            }
        }

        emit dataUpdated();

        // 每分钟更新一次
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}
