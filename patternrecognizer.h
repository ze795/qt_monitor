#ifndef PATTERNRECOGNIZER_H
#define PATTERNRECOGNIZER_H


#include <QString>

class PatternRecognizer
{
public:
    static bool recognizePattern(const QString& symbol, const QString& direction);
};

#endif // PATTERNRECOGNIZER_H
