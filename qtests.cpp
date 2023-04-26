#include "qtests.h"

void MathUtilsTester::testOperators()
{
    //'Plus' operator testing
    QCOMPARE(m_parser.calculateExpression("1+ 2"), 3);
    QCOMPARE(m_parser.calculateExpression("5+2-(-5)"), 12);
    QCOMPARE(m_parser.calculateExpression("1+2+2+2"), 7);
}
