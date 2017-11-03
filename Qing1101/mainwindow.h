#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <opencv2/opencv.hpp>//声明函数mat的显示要用到这个形参，并且要用cv::
#include<QLabel>//声明函数mat的显示要用到这个形参,
#include <QtNetwork>

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

    QUdpSocket *read;//因为要在槽函数使用，因此这里在头文件进行定义
    QLabel  *receivedlabel;


public slots:
    void readData();//槽函数用于对接受的数据进行处理

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
    void on_Features_dect_clicked();
    //void on_spinBox_editingFinished();
    void on_minHessian_editingFinished();
    void on_minHessian_valueChanged(int arg1);
};
#endif // MAINWINDOW_H
