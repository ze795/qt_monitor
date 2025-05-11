// futuresitem.cpp
#include "futuresitem.h"

FuturesItem::FuturesItem(int _id, const QString& _name)
    : id(_id), name(_name), currentPrice(0.0), monitorPrice(0.0),
      priceMonitorEnabled(false), atrDirection("多"), atrMonitorEnabled(false),
      patternDirection("多"), patternMonitorEnabled(false),
      status("正常"), statusColor(Qt::black)
{
}
