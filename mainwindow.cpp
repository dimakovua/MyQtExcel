#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <parser.h>
#include <iostream>
#include <fstream>
#include <vector>
#include  <random>
#include  <iterator>

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
    for (int i = 0; i< ui->tableWidget->rowCount() ; i++ ) {
            for(int j  = 0; j < ui->tableWidget->columnCount(); j++){
                QTableWidgetItem* itm = new QTableWidgetItem(" ");
                ui->tableWidget->setItem(i, j, itm);
            }
    }
    ChangeExpressions();
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
    if(is_double_clicked){
        UpdateText(row, column);
        //is_double_clicked = 0;
    }
    //ChangeExpressions();
}
void MainWindow::UpdateText(int row, int column){
    if(row == changed_by_hands.first && column == changed_by_hands.second){
       // std::cout << "UpdateText! " << changed_by_hands.first << " " << changed_by_hands.second << "\n";
        expressions[row][column] = ui->tableWidget->item(row, column)->text();
        changed_by_hands.first = -1;
        changed_by_hands.second = -1;
    }
}
void MainWindow::ChangeExpressions(){
    std::cout <<"Change Expression!\n";
    int i = ui->tableWidget->rowCount();
    int j = ui->tableWidget->columnCount();
   // std::cout << "i = " <<i << " j = " << j << "\n";
    expressions.resize(i, vector<QString>(j));
    for(int a = 0; a < i; a++){
        for(int b = 0; b < j; b++){
            expressions[a][b] = ui->tableWidget->item(a, b)->text();
        }
    }
}
void MainWindow::on_CalculateButton_clicked()
{
    changed_by_hands.first = -1;
    changed_by_hands.second = -1;
    changePic();
    Parser p;
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    int n = 0;


    p.clearEntranceTable();
    double res = p.RecursiveRef(item, ui->tableWidget, n);
    cerr << fixed << res << '\n';
    if(res == CODE_NUMBER_FOR_CYCLE) {
        QMessageBox::critical(this, "Error!", "Cycle!");
    }
    else if(res == CODE_NUMBER_FOR_BAD_EXPRESSION)
    {
        QMessageBox::critical(this, "Error!", "Bad expression!");
    }
    else
    {
        QString answer = QString::number(res);
        item->setText(answer);
    }
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
    for (int i = 0; i< ui->tableWidget->rowCount() ; i++ ) {
            for(int j  = 0; j < ui->tableWidget->columnCount(); j++){
                std::cerr << ui->tableWidget->item(i, j)->text().toStdString() << "|";
                //out << ui->tableWidget->item(i, j)->text().toStdString() << "|";
                out << expressions[i][j].toStdString() << "|";
            }
    }
    out.close();
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


void MainWindow::on_tableWidget_cellPressed(int row, int column)
{
    is_double_clicked = 1;
    changed_by_hands.first = row;
    changed_by_hands.second = column;
    //std::cout << "Is double clicked\n";
    //std::cout << "Double clicked on " << changed_by_hands.first << " " << changed_by_hands.second << std::endl;
}


void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    ui->tableWidget->currentItem()->setText(arg1);
    changed_by_hands.first = ui->tableWidget->currentItem()->row();
    changed_by_hands.second = ui->tableWidget->currentItem()->column();
    UpdateText(changed_by_hands.first, changed_by_hands.second);
}

