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
//#include <bits/stdc++.h>
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

}


void MainWindow::on_CalculateButton_clicked()
{

    changePic();
    Parser p;
    QTableWidgetItem* item = ui->tableWidget->currentItem();
    int n = 0;
    int res = p.RecursiveRef(item, ui->tableWidget, n);
    if(res == -10000 || res == -2147483648)
    {
        QMessageBox::critical(this, "Error!", "Bad expression!");
    }
    else
    {
        QString answer = QString::number(res);
        item->setText(answer);
    }
}



void MainWindow::on_SetAboba_clicked()
{
    changePic();
    for (int i = 0; i< ui->tableWidget->rowCount() ; i++ ) {
            for(int j  = 0; j < ui->tableWidget->columnCount(); j++){
                QTableWidgetItem* itm = new QTableWidgetItem(" ");
                ui->tableWidget->setItem(i, j, itm);
            }
    }
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
                out << ui->tableWidget->item(i, j)->text().toStdString() << "|";
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

