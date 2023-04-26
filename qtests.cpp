#include "qtests.h"

void MathUtilsTester::testOperators_data()
{
    QTest::addColumn<string>("expression");
    QTest::addColumn<double>("result");

    QTest::newRow("Plus") << std::string("1+2") << 3.0;
    QTest::newRow("Plus (difficult)") << std::string("5+2-(-5)") << 12.0;

    QTest::newRow("Minus") << std::string("1-2") << -1.0;
    QTest::newRow("Minus (difficult") << std::string("1 - (-5)") << 6.0;

    QTest::newRow("Divide") << std::string("10/5") << 2.0;
    QTest::newRow("Divide zero") << std::string("1/0") << CODE_NUMBER_FOR_BAD_EXPRESSION;
    QTest::newRow("Divide (difficult)") << std::string("-1/(-1)") << 1.0;

    QTest::newRow("Invalid") << std::string("Aboba/?") << CODE_NUMBER_FOR_BAD_EXPRESSION;

    QTest::newRow("Mod") << std::string("6 mod 2") << 0.0;
    QTest::newRow("Mod zero") << std::string("6 mod 0") << CODE_NUMBER_FOR_BAD_EXPRESSION;

    QTest::newRow("Spaces") << std::string("1      +      2") << 3.0;
}


void MathUtilsTester::testOperators()
{
    QFETCH(string, expression);
    QFETCH(double, result);
    QCOMPARE(m_parser.calculateExpression(expression), result);
}
