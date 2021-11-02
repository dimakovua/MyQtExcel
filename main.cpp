#include "mainwindow.h"
#include "test_runner.h"
#include "parser.h"
#include <QApplication>
void TestOperatorsPlus(){
    Parser p;
    ASSERT_EQUAL(p.calculateExpression("1+ 2"), 3);
    ASSERT_EQUAL(p.calculateExpression("5+2-(-5)"), 12);
    ASSERT_EQUAL(p.calculateExpression("1+2+2+2"), 7);
}
void TestOperatorsMinus(){
    Parser p;
    ASSERT_EQUAL(p.calculateExpression("1 - (-5)"), 6);
    ASSERT_EQUAL(p.calculateExpression("-5+2"), -3);
    ASSERT_EQUAL(p.calculateExpression("-1^2"), 1);
}
void TestOperatorsDivide(){
    Parser p;
    ASSERT_EQUAL(p.calculateExpression("1/0"), -10000);
    ASSERT_EQUAL(p.calculateExpression("1/"), -10000);
    ASSERT_EQUAL(p.calculateExpression("-1/1"), -1);
    ASSERT_EQUAL(p.calculateExpression("-1/(-1)"), 1);
}
void TestInvalidArguments(){
    Parser p;
    ASSERT_EQUAL(p.calculateExpression("Aboba"), -10000);
    ASSERT_EQUAL(p.calculateExpression("11?/"), -10000);
}
void TestMod(){
    Parser p;
    ASSERT_EQUAL(p.calculateExpression("6 mod 2"), 0);
    ASSERT_EQUAL(p.calculateExpression("6 mod 4"), 2);
    ASSERT_EQUAL(p.calculateExpression("6 mod 5"), 1);
    //std::cerr << "ok";
    ASSERT_EQUAL(p.calculateExpression("6 mod"), -10000);
    ASSERT_EQUAL(p.calculateExpression("6 mod 0"), -10000);
}
void TestPow(){
    Parser p;
    ASSERT_EQUAL(p.calculateExpression("6^0"), 1);
    ASSERT_EQUAL(p.calculateExpression("6^1"), 6);
    ASSERT_EQUAL(p.calculateExpression("2^(-1)"), 0.5);
    ASSERT_EQUAL(p.calculateExpression("6^"), -10000);
}
void TestDiv(){
    Parser p;
    ASSERT_EQUAL(p.calculateExpression("1div0"), -10000);
    ASSERT_EQUAL(p.calculateExpression("1div"), -10000);
    ASSERT_EQUAL(p.calculateExpression("(-1)div1"), -1);
    ASSERT_EQUAL(p.calculateExpression("(-1)div(-1)"), 1);
}
void TestSpaces(){
    Parser p;
    ASSERT_EQUAL(p.calculateExpression("1      +      2"), 3);
}
void TestAll(){
    TestRunner tr;
    RUN_TEST(tr, TestSpaces);
    RUN_TEST(tr, TestOperatorsPlus);
    RUN_TEST(tr, TestOperatorsMinus);
    RUN_TEST(tr, TestOperatorsDivide);
    RUN_TEST(tr, TestInvalidArguments);
    RUN_TEST(tr, TestMod);
    RUN_TEST(tr, TestPow);
    RUN_TEST(tr, TestDiv);
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    TestAll();
    w.show();
    return a.exec();
}
