#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QTabWidget>
#include <vector>
#include <QPixmap>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_helper_triggered();

    void on_actionQuit_triggered();

    void on_tableWidget_cellChanged(int row, int column);

    void on_CalculateButton_clicked();

    void on_SetAboba_clicked();

    void on_SaveFile_clicked();

    void on_OpenFile_clicked();

private:
    void changePic();
    std::vector<QPixmap> pixes;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
