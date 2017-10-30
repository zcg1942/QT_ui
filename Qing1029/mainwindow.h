#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <opencv2/opencv.hpp>//声明函数mat的显示要用到这个形参，并且要用cv::
#include<QLabel>//声明函数mat的显示要用到这个形参,

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QLabel * label;
    void MainWindow::paintEvent(QPaintEvent *event);//声明paintevent，用于背景颜色
    void MainWindow::displayResult(cv::Mat mat,QLabel * label);



private:
    Ui::MainWindow *ui;

//编写槽函数
private slots:
    void on_OpenFile();
    void on_START_clicked();
    void on_GaussianBlur_clicked();
//    void on_START_clicked();//begin开始图像处理
//    void MainWindow::on_GaussianBlur_clicked();转到槽就自动声明了，不用画蛇添足了

    void on_medianBlur_clicked();
    void on_Save_clicked();
    void on_Send_clicked();
};
#endif // MAINWINDOW_H
