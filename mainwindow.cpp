#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <parser.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include  <random>
#include  <iterator>
#include <QStringList>

template<typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g) {
    std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
    std::advance(start, dis(g));
    return start;
}

template<typename Iter>
Iter select_randomly(Iter start, Iter end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return select_randomly(start, end, gen);
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableWidget->blockSignals(1);
    std::cerr <<"signals in constructor are blocked!" << std::endl;
    for (int i = 0; i< ui->tableWidget->rowCount() ; i++ ) {
            for(int j  = 0; j < ui->tableWidget->columnCount(); j++){
                QTableWidgetItem* itm = new QTableWidgetItem(" ");
                ui->tableWidget->setItem(i, j, itm);
            }
    }
    std::cerr << "cycle is done!" << std::endl;
    ChangeExpressions();
    std::cerr << "Change expr is done!" << std::endl;
    QLabel label;
    QPixmap pix("/Users/dmitrikovalenko/MyExcel/Pics/dog.png");
    QPixmap pix2("/Users/dmitrikovalenko/MyExcel/Pics/frog.png");
    QPixmap pix3("/Users/dmitrikovalenko/MyExcel/Pics/sad_dog.png");
    QPixmap pix4("/Users/dmitrikovalenko/MyExcel/Pics/knife.png");
    pixes = {pix, pix2, pix3, pix4};
    ui->label->setPixmap(pix);
    ui->label->show();
    QTableWidgetItem* cur = ui->tableWidget->item(0,0);
    ui->tableWidget->setCurrentItem(cur);
    //std::cout << "Constructor done";
    ui->tableWidget->blockSignals(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_helper_triggered()
{
    changePic();
    QMessageBox::about(this, "Helper", "Operators: + - / * ^ max min mod\nYou can use references (A1+B2), but be carefull with cycled references)\nType name of file and choose Save/Open\nby Dmytro Kovalenko");
}


void MainWindow::on_actionQuit_triggered()
{
    this->close();
}


void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
        UpdateText(row, column);
}

void MainWindow::UpdateText(int row, int column){
   expressions[row][column] = ui->tableWidget->item(row, column)->text();
}
void MainWindow::ChangeExpressions(){
    QString space = " ";
    std::cout <<"Change Expression!\n";
    int i = ui->tableWidget->rowCount();
    int j = ui->tableWidget->columnCount();
   // std::cout << "i = " <<i << " j = " << j << "\n";
    expressions.resize(i, vector<QString>(j));
    for(int a = 0; a < i; a++){
        for(int b = 0; b < j; b++){
            if(ui->tableWidget->item(a,b)->text() != space){
                expressions[a][b] = ui->tableWidget->item(a, b)->text();
                std::cout << expressions[a][b].toStdString() << " ";
            }
            else{
                expressions[a][b] = "0";
                std::cout << expressions[a][b].toStdString() << " ";
            }
        }
        std::cout << std::endl;
    }
}

void MainWindow::on_CalculateButton_clicked()
{
    ui->tableWidget->blockSignals(1);
    changed_by_hands.first = -1;
    changed_by_hands.second = -1;
    changePic();
    Parser p;
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    int n = 0;


    p.clearEntranceTable();
    double res = p.RecursiveRef(this, item, ui->tableWidget, n);
   // p.Printtable();
    cerr << fixed << res << '\n';
    if(res == CODE_NUMBER_FOR_CYCLE) {
        QMessageBox::critical(this, "Error!", "Cycle!");
    }
    else if(res == CODE_NUMBER_FOR_BAD_EXPRESSION)
    {
        QMessageBox::critical(this, "Error!", "Bad expression!");
    }
    else if(res == CODE_NUMBER_FOR_MY_STICK)
    {
        QMessageBox::critical(this, "Error!", "Please! Don't use \'|\', because I use it for saving files)");
    }
    else
    {
        QString answer = QString::number(res);
        item->setText(answer);
    }
    ui->tableWidget->blockSignals(0);
}
void MainWindow::CleanTable(){
    for (int i = 0; i< ui->tableWidget->rowCount() ; i++ ) {
            for(int j  = 0; j < ui->tableWidget->columnCount(); j++){
                QTableWidgetItem* itm = new QTableWidgetItem(" ");
                ui->tableWidget->setItem(i, j, itm);
            }
    }
    ChangeExpressions();
}
void MainWindow::on_SetAboba_clicked()
{
    CleanTable();
    changePic();
}

void MainWindow::on_SaveFile_clicked()
{
    changePic();
    string file_path = "/Users/dmitrikovalenko/MyExcel/";
    file_path += ui->file_name->text().toStdString();
    ofstream out;
    out.open(file_path);
    if(out.is_open()){
        std::cerr << "ABOBA";
    }
    string line;
    bool bad = 0;
    for (int i = 0; i< ui->tableWidget->rowCount() ; i++ ) {
        if(bad){break;}
            for(int j  = 0; j < ui->tableWidget->columnCount(); j++){
                line = ui->tableWidget->item(i, j)->text().toStdString();
                if(line.find('|') != string::npos){
                    QMessageBox::critical(this, "Error!", "Please! Don't use \'|\', because I use it for saving files)");
                    out.clear();
                    bad = 1;
                    out.close();
                    out.open(file_path);
                    out.close();
                    break;
                }
                std::cerr << line << "|";
                //out << ui->tableWidget->item(i, j)->text().toStdString() << "|";
                out << expressions[i][j].toStdString() << "|";
            }
    }
    out.close();
}
QString MainWindow::GetExpr(int row, int col){
    return expressions[row][col];
}
void MainWindow::changePic(){
    ui->label->setPixmap(*select_randomly(pixes.begin(), pixes.end()));
    ui->label->show();
}
void MainWindow::on_OpenFile_clicked()
{
    changePic();
    CleanTable();
    string data;
    string file_path = "/Users/dmitrikovalenko/MyExcel/";
    ifstream in;
    if(ui->file_name->text() != ""){
    file_path += ui->file_name->text().toStdString();

    in.open(file_path);}
    if(!in.is_open()){
        QMessageBox::critical(this, "Error!", "Unknown file!");
    }
    else{
        getline(in, data);
        if(data.length() == 0){
            QMessageBox::critical(this, "Error!", "Empty file! Did you try to save table with \'|\'?");
        }
        else{
            //std::cerr << data;
            vector<string> data_splitted;
            string word;
            for(int i =0; i < data.length(); i++){
                if(data[i] != '|'){
                    word+=data[i];
                }
                else{
                    data_splitted.push_back(word);
                    word = "";
                }
            }
            int d = 0;
    //        for(auto aa : data_splitted){
    //            std::cerr << aa;
    //        }
            for (int i = 0; i< ui->tableWidget->rowCount() ; i++ ) {
                    for(int j  = 0; j < ui->tableWidget->columnCount(); j++){
                        QTableWidgetItem* itm = new QTableWidgetItem(QString::fromStdString(data_splitted[d]));
                        d++;
                        ui->tableWidget->setItem(i, j, itm);
                    }
            }
            ChangeExpressions();
        }
    }
}


void MainWindow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    //ChangeExpressions();
    //std::cout << "new row: " << currentRow << " new col: " << currentColumn << "\n";
    ShowText(currentRow, currentColumn);
}
void MainWindow::ShowText(int row, int column){
    ui->lineEdit->setText(expressions[row][column]);
}

void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    ui->tableWidget->currentItem()->setText(arg1);
    changed_by_hands.first = ui->tableWidget->currentItem()->row();
    changed_by_hands.second = ui->tableWidget->currentItem()->column();
    UpdateText(changed_by_hands.first, changed_by_hands.second);
}


void MainWindow::on_actionAdd_row_2_triggered()
{
    if(ui->tableWidget->rowCount()==9){
        QMessageBox::critical(this, "Error!","Unable to add row!");
    }
    else{
    ui->tableWidget->insertRow(1);
    std::cout << "done!" << std::endl;
    ui->tableWidget->setVerticalHeaderLabels(QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10");
    //this->ChangeExpressions();
    }
}



void MainWindow::on_actionAdd_column_2_triggered()
{
    if(ui->tableWidget->columnCount()==9){
        QMessageBox::critical(this, "Error!","Unable to add column!");
    }
    else{
        ui->tableWidget->insertColumn(1);
        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I");
        //this->ChangeExpressions();
    }
}


void MainWindow::on_actionRemove_row_triggered()
{
    ui->tableWidget->removeRow(1);
    ui->tableWidget->setVerticalHeaderLabels(QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9");
    //this->ChangeExpressions();
}


void MainWindow::on_actionRemove_column_triggered()
{
    ui->tableWidget->removeColumn(1);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I");
    //this->ChangeExpressions();
}


