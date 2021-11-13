#include "parser.h"
#include <iostream>
double Parser::RecursiveRef(QTableWidgetItem* item, QTableWidget* table,int& number_of_iterations){
    std::cerr << "rec started\n";
    if(number_of_iterations > 100){
        std::cerr << "Wow! Infinite recursion\n";
        return -2147483648;
    }
    if(item->text() == " "){
        return -2147483648;
    }
    number_of_iterations++;
    QString line = item->text();
    string line_str = line.toStdString();
    string result;
    //std::cerr << "\nline_str " << line_str << "\n";

    for(int i = 0; i < line_str.length(); i++)
    {
       // std::cerr << "cycle started\n";
//        if(line_str[i] == ' ')
//        {
//            i--;
//        }
        if((int)line_str[i] >= 65 && (int)line_str[i] <=71)
        {
          //  std::cerr << "we are herre!\n";
            int column_of_ref = (int)line_str[i]-65;

            string row;
            row +=line_str[i+1];
            int row_of_ref =0;
            try
            {
                row_of_ref= stoi(row) -1;
                if(row_of_ref >= table->rowCount()){
                    throw std::invalid_argument("Out of range");
                }
            }
            catch(invalid_argument)
            {
                std::cerr<<"Invalid argument\n";
                QMessageBox::critical(table, "Error", "Invalid Argument");
            }
            QTableWidgetItem* ref_item = table->item(row_of_ref, column_of_ref);
            double value_of_ref = RecursiveRef(ref_item, table, number_of_iterations);
            if(value_of_ref == -2147483648){
                return -2147483648;
            }
            result+=to_string(value_of_ref);
            i++;
        }
        else
        {
            //std::cerr << "else\n";
            result+= line_str[i];
        }
    }
    std::cerr << "Result to calculate: " << result;
    return calculateExpression(result);
}


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
        if(s[i] == '-' && (i == 0 || s[i-1] == '(' || s[i-1] == '+'||s[i-1] == '/'||s[i-1] == '-'||s[i-1] == '*')){
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
        else if(s[i] == 'm' || s[i] == 'd') {
            string tmp(s.begin()+i, s.begin()+i+3);
            result.push_back(tmp);
            i+=2;
        }
        else {
            if(to_reverse_next_number ==1){
                resNumber+= 'q';
                to_reverse_next_number = 0;
            }
            resNumber+=s[i];
        }
    }
    if(!resNumber.empty()) result.push_back(resNumber);
    return result;
}

map<string, int> priorities = {{"+", 1},{"-", 1},{"*", 2},{"/", 2},{"^", 3} ,{"mod", 2}, {"div", 2}};
double Parser::calculateExpression(const string& inputExpression) {
    vector<string> tokens = parseExpression(inputExpression);
    //for(auto it: tokens) std::cerr << it << " ";
    //std::cerr<<"\n";
    tokens.emplace_back("X");
    vector<double> numbersStack;
    vector<string> operationsStack;
    map<std::string, std::function<double(const double&, const double&)>> operations;
    operations["+"] = [](double a, double b) {return a+b;};
    operations["-"] = [](double a, double b) {return a-b;};
    operations["*"] = [](double a, double b) {return a*b;};
    operations["/"] = [](double a, double b) {
        if(b == 0) throw ("division by zero!");
        return a/b;
    };
    operations["^"] = [](double a, double b) {return pow(a,b);};
    operations["mod"] = [](double a, double b) {
        if (b == 0) throw "division by zero!";
        return (int)a%(int)b;
    };

    operations["div"] = [](double a, double b) {
        if(b == 0) throw ("division by zero!");
        return (int)a/(int)b;
    };
    operations["max"] = [](double a, double b) {return max(a,b);};
    operations["min"] = [](double a, double b) {return min(a,b);};

    for(auto it: tokens) cerr << it << " ";
    std::cerr << '\n';
    for(auto it: tokens)
    {
        if(isInteger(it))
        {
            string reverted;
            if(it[0] == 'q')
            {
                for(int i = 1; i < it.length(); i++)
                {
                    reverted+=it[i];
                }
                double num = (atof(reverted.c_str())*(-1));
                numbersStack.push_back(num);

            }
            else
            {
            numbersStack.push_back(atof(it.c_str()));
            }
        }

        else
        {
            if(isOperation(it))
            {
                while(!operationsStack.empty() && priorities[operationsStack.back()] >= priorities[it])
                {
                    std::string OpBack = operationsStack.back();
                    int OpBackPrior = priorities[OpBack];
                    int itPrior = priorities[it];
                    if(operationsStack.back() == "(" || operationsStack.back() == ")") break;
                    double secondOperand = numbersStack.back();
                    numbersStack.pop_back();
                    double firstOperand;
                    try{
                        if(numbersStack.size() == 1 && (it == "mod" || it == "div" || it == "*" || it == "/" || it == "^")){
                            throw std::invalid_argument("one argument only!");
                        }
                        firstOperand = numbersStack.back();}
                    catch(const invalid_argument ex){
                        return -10000;
                    }

                    numbersStack.pop_back();
                    numbersStack.push_back(operations[operationsStack.back()](firstOperand, secondOperand));
                    operationsStack.pop_back();
                }
                operationsStack.push_back(it);
            }
            else if(it == "(") operationsStack.push_back(it);

            else if(it == ")")
            {
                while(operationsStack.back() != "(")
                {
                    double secondOperand = numbersStack.back();
                    numbersStack.pop_back();
                    double firstOperand = numbersStack.back();
                    numbersStack.pop_back();
                    numbersStack.push_back(operations[operationsStack.back()](firstOperand, secondOperand));
                    operationsStack.pop_back();
                }
                operationsStack.pop_back();
            }
            else if(it == "X")
            {
                if(numbersStack.size() - operationsStack.size() == 1)
                {
                    while(!operationsStack.empty())
                    {
                        double secondOperand = numbersStack.back();
                        numbersStack.pop_back();
                        double firstOperand = numbersStack.back();
                        numbersStack.pop_back();
                        try
                        {
                        numbersStack.push_back(operations[operationsStack.back()](firstOperand, secondOperand));
                        }
                        catch(const std::exception& ex)
                        {
                            std::cerr<<ex.what();
                        }

                        operationsStack.pop_back();
                    }
                    return numbersStack.back();
                }
                else
                {
                    return -10000;
                }
            }
        }
    }
    return numbersStack.back();
}
