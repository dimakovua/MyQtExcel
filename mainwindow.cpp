#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <parser.h>
#include <iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for (int i = 0; i< ui->tableWidget->rowCount() ; i++ ) {
            for(int j  = 0; j < ui->tableWidget->columnCount(); j++){
                QTableWidgetItem* itm = new QTableWidgetItem("");
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

int RecursiveRef(QTableWidgetItem* item, QTableWidget* table){
    QString line = item->text();
    string line_str = line.toStdString();
    std::cerr << "line_str = " << line_str << "\n";
    string result;
    std::cerr << "result = " << result << "\n";
    for(int i = 0; i < line_str.length(); i++){
        std::cerr << "i = " << i << "\n";
        if(line_str[i] == ' '){
            i--;
        }
        else if((int)line_str[i] <=71 && (int)line_str[i] >= 65){
            std::cerr << "line_str[i] " << line_str[i] << "\n";
            int column_of_ref = (int)line_str[i]-65;
            std::cerr <<"column_of_ref = " << column_of_ref << "\n";
            string row;
            row +=line_str[i+1];
            std::cerr << "string row: " << row << "\n";
            int row_of_ref = stoi(row) -1;
            std::cerr << "row_of_ref = " << row_of_ref << "\n";
            QTableWidgetItem* ref_item = table->item(row_of_ref, column_of_ref);
            std::cerr << "We got new item with text: " << ref_item->text().toStdString() << "\n";
            std::cerr << "Sending new item to new recursion..\n";
            int value_of_ref = RecursiveRef(ref_item, table);
            std::cerr <<"Got value_of_ref = "<<value_of_ref << "\n";

            result+=to_string(value_of_ref);
            std::cerr << "New result: " << result << "\n";
            i++;
        }
        else{
            std::cerr << "ELSE\n";
            result+= line_str[i];
            std::cerr << "Else result: "<<result <<"\n";
        }
    }
    Parser par;
    return par.calculateExpression(result);

}
void MainWindow::on_CalculateButton_clicked()
{
        QTableWidgetItem* item = ui->tableWidget->currentItem();
        //QString str = item->text();
        //Parser p;
        //string line = str.toStdString();
//        for(int i = 0; i < line.length(); i++){
//            if((int)line[i] <= 71 && (int)line[i] >= 65){
//                int column_of_ref = i-65;

//            }
//        }
//        for(int i = 0; i < line.length(); i++){
//            if((int)line[i] <= 71 && (int)line[i] >= 65){

//            }
//        }
        QString answer = QString::number(RecursiveRef(item, ui->tableWidget));
        item->setText(answer);

}



void MainWindow::on_SetAboba_clicked()
{
    for (int i = 0; i< ui->tableWidget->rowCount() ; i++ ) {
            for(int j  = 0; j < ui->tableWidget->columnCount(); j++){
                QTableWidgetItem* itm = new QTableWidgetItem("Aboba");
                ui->tableWidget->setItem(i, j, itm);
            }
    }
}

