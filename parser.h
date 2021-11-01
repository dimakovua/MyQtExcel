#ifndef TESTPARSER_PARSER_H
#define TESTPARSER_PARSER_H
#include<stack>
#include<vector>
#include<map>
#include<cmath>
#include<algorithm>
#include<functional>
#include<string>
using namespace std;
class Parser {
public:

    bool isInteger(const std::string & s);
    bool isDigit(char a);
    bool isOperation(const string& s);
    bool isOperation(char s);
    std::vector<std::string> splitString(const std::string &str);
    vector<string> parseExpression(const string& s);
    vector<string> findRef(const string& s);
    double calculateExpression(const string& inputExpression);
private:
    map<string, int> priorities = {{"+", 1},{"-", 1},{"*", 2},{"/", 2},{"^", 3} ,{"mod", 2}, {"div", 2}};
};
#endif //TESTPARSER_PARSER_H
