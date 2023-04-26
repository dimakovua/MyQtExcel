#ifndef QTESTS_H
#define QTESTS_H

#include <QObject>
#include <QTest>
#include <parser.h>

class MathUtilsTester : public QObject
{
    Q_OBJECT
private slots:
    void testOperators();
private:
    Parser m_parser;
};

#endif // QTESTS_H
