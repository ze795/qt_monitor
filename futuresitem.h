#ifndef FUTURESITEM_H
#define FUTURESITEM_H

#include <QString>
#include <QColor>

struct FuturesItem {
    int id;
    QString name;
    double currentPrice;
    double monitorPrice;
    bool priceMonitorEnabled;
    QString atrDirection; // "多" 或 "空"
    bool atrMonitorEnabled;
    QString patternDirection; // "多" 或 "空"
    bool patternMonitorEnabled;
    QString status;
    QColor statusColor;

    FuturesItem(int _id, const QString& _name);
};

#endif // FUTURESITEM_H
