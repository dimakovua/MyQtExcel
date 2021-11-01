#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <parser.h>
#include <iostream>
#include <fstream>
#include <vector>
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
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_helper_triggered()
{
    QMessageBox::about(this, "Helper", "I will write something here later(");
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
    for (int i = 0; i< ui->tableWidget->rowCount() ; i++ ) {
            for(int j  = 0; j < ui->tableWidget->columnCount(); j++){
                QTableWidgetItem* itm = new QTableWidgetItem(" ");
                ui->tableWidget->setItem(i, j, itm);
            }
    }
}





void MainWindow::on_SaveFile_clicked()
{
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


void MainWindow::on_OpenFile_clicked()
{
    string data;
    string file_path = "/Users/dmitrikovalenko/MyExcel/";
    file_path += ui->file_name->text().toStdString();
    ifstream in;
    in.open(file_path);
    if(!in.is_open()){
        QMessageBox::critical(this, "Error!", "Unknown file!");
    }
    else{
        getline(in, data);
        std::cerr << data;
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
        for(auto aa : data_splitted){
            std::cerr << aa;
        }
        for (int i = 0; i< ui->tableWidget->rowCount() ; i++ ) {
                for(int j  = 0; j < ui->tableWidget->columnCount(); j++){
                    QTableWidgetItem* itm = new QTableWidgetItem(QString::fromStdString(data_splitted[d]));
                    d++;
                    ui->tableWidget->setItem(i, j, itm);
                }
        }
    }
}

