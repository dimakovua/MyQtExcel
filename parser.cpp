#include "parser.h"
#include <iostream>
bool Parser::isInteger(const std::string & s)
{
    if(s[0] == 'q') return true;
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;


    char * p;
    strtol(s.c_str(), &p, 10);

    return (*p == 0);
}

bool Parser::isDigit(char a) {
    string tmp = "0123456789";
    for(auto it: tmp) if(it == a) return true;
    return false;
}
bool Parser::isOperation(const string& s) {
    return s == "*" || s == "/" || s == "-" || s == "+" || s == "^" || s == "mod" || s == "div" || s == "max" || s == "min";
}
bool Parser::isOperation(char s) {
    return s == '*' || s == '/' || s == '+' || s == '^' || s == '-';
}
std::vector<std::string> Parser::splitString(const std::string &str) {
    std::string word;
    std::vector<std::string> result;
    for (auto x : str) {
        if (x == ' ') {
            result.push_back(word);
            result.push_back(" ");
            word = "";
        }
        else word = word + x;
    }
    result.push_back(word);
    return result;
}
vector<string> findRef(const string &s){
    string beforeRef;
    for(int i = 0; i < s.length(); i++){
        if((int)s[i] <= 71 && (int)s[i] >= 65){

        }
    }
}

vector<string> Parser::parseExpression(const string& s) {
    vector<string> result;
    bool to_reverse_next_number = 0;
    string resNumber;
    for(int i = 0; i < s.size(); ++i) {
        if(s[i] == '-' && (i == 0 || s[i-1] == '(')){
                to_reverse_next_number = 1;
        }
        else if(s[i] == ' ' || s[i] == '(' || s[i] == ')' || isOperation(s[i])) {
            if(!resNumber.empty()) {
                result.push_back(resNumber);
                resNumber = "";
            }
            if(s[i] == ' ') continue;
            string tmp(1, s[i]);
            result.push_back(tmp);

        }
        else if(s[i] == 'm' || s[i] == 'd') { // значит текущее слово это min || max || mod || div
            string tmp(s.begin()+i, s.begin()+i+3);
            result.push_back(tmp);
            i+=2;
        }
        else {
            if(to_reverse_next_number ==1){
                resNumber+= 'q';
                to_reverse_next_number = 0;
            }//если цифра
            resNumber+=s[i];
        }
    }
    if(!resNumber.empty()) result.push_back(resNumber);
    return result;
}
//template<class T>
//void printVector(const vector<T>& vec) {
//    std::cout << '\n';
//    for(const auto& itt: vec) std::cout << itt << " ";
//    std::cout << '\n';
//}
map<string, int> priorities = {{"+", 1},{"-", 1},{"*", 2},{"/", 2},{"^", 3} ,{"mod", 2}, {"div", 2}};
double Parser::calculateExpression(const string& inputExpression) {
    vector<string> tokens = parseExpression(inputExpression);
    for(auto it: tokens) std::cerr << it << " ";
    std::cerr<<"\n";
    tokens.emplace_back("X");
    vector<double> numbersStack;
    vector<string> operationsStack;
    map<std::string, std::function<double(const double&, const double&)>> operations;
    operations["+"] = [](double a, double b) {return a+b;};
    operations["-"] = [](double a, double b) {return a-b;};
    operations["*"] = [](double a, double b) {return a*b;};
    operations["/"] = [](double a, double b) {return a/b;};
    operations["^"] = [](double a, double b) {return pow(a,b);};
    operations["mod"] = [](double a, double b) {return (int)a%(int)b;};
    operations["div"] = [](double a, double b) {return (int)a/(int)b;};
    operations["max"] = [](double a, double b) {return max(a,b);};
    operations["min"] = [](double a, double b) {return min(a,b);};
    for(auto it: tokens) {
        if(isInteger(it)) {
            //std::cerr << "ABOBA\n";
            string reverted;
            //std::cerr << it[0] <<"\n";
            if(it[0] == 'q'){
                //std::cerr << "We are in if\n";
                for(int i = 1; i < it.length(); i++){
                    reverted+=it[i];
                }
                //std::cerr << reverted << "\n";
                double num = (atof(reverted.c_str())*(-1));
                //std::cerr << "New num = " << num << "\n";
                numbersStack.push_back(num);

            }
            else{
            numbersStack.push_back(atof(it.c_str()));}
        }

else {
            if(isOperation(it)) {
                while(!operationsStack.empty() && priorities[operationsStack.back()] >= priorities[it]) {
                    std::string OpBack = operationsStack.back();
                    int OpBackPrior = priorities[OpBack];
                    int itPrior = priorities[it];
                    if(operationsStack.back() == "(" || operationsStack.back() == ")") break;
                    double secondOperand = numbersStack.back();
                    numbersStack.pop_back();
                    double firstOperand = numbersStack.back();
                    numbersStack.pop_back();
                    numbersStack.push_back(operations[operationsStack.back()](firstOperand, secondOperand));
                    operationsStack.pop_back();
                }
                operationsStack.push_back(it);
            }
            else if(it == "(") operationsStack.push_back(it);

            else if(it == ")") {
                while(operationsStack.back() != "(") {
                    double secondOperand = numbersStack.back();
                    numbersStack.pop_back();
                    double firstOperand = numbersStack.back();
                    numbersStack.pop_back();
                    numbersStack.push_back(operations[operationsStack.back()](firstOperand, secondOperand));
                    operationsStack.pop_back();
                }
                operationsStack.pop_back();
            }
            else if(it == "X") {
                //printVector(operationsStack);
                //printVector(numbersStack);
                //проверка на правильность арифметического выражения
//                for(auto it: numbersStack) qDebug() << it << " ";
//                qDebug() << '\n';
//                for(auto it: operationsStack) std::cout << it << " ";
//                qDebug() << '\n';
                if(numbersStack.size() - operationsStack.size() == 1) { // должно быть так(наверное)
                    while(!operationsStack.empty()) {
                        double secondOperand = numbersStack.back();
                        numbersStack.pop_back();
                        double firstOperand = numbersStack.back();
                        numbersStack.pop_back();
                        try{
                        numbersStack.push_back(operations[operationsStack.back()](firstOperand, secondOperand));}
                        catch(const std::exception& ex){
                            std::cerr<<ex.what();
                        }

                        operationsStack.pop_back();
                    }
                    return numbersStack.back();
                }
                else {
                    return -10000;
                }
            }
        }
    }
    return numbersStack.back();
}
