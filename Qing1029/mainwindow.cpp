#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <opencv2/opencv.hpp>
#include <QFileDialog>
#include <QDebug>
#include <QString>
#include<QMessageBox>

#include <opencv2/opencv.hpp>
#include <QtNetwork>//qt网络编程
using namespace std;
using namespace cv;

//声明全局变量
 //QPixmap image; //设置成全局变量会中断，提示Must construct a QGuiApplication before a QPixmap
  QString file_name;
  Mat src;
  Mat dst;
  int key;

 QString Qdstpath;
//  enum blur{GaussianBlur,medianBlur};
//   enum blur key; 还是整型好用


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   ui->actionOpen_O->setIcon(QIcon(":/Open.png")); // Add code.
    connect(ui->actionOpen_O,SIGNAL(triggered()),this,SLOT(on_OpenFile()));//连接信号与槽
     connect(ui->GaussianBlur,SIGNAL(stateChanged(int)),this,SLOT(on_GaussianBlur_clicked()));//连接信号与槽
     connect(ui->medianBlur,SIGNAL(stateChanged(int)),this,SLOT(on_medianBlur_clicked()));//连接信号与槽
     connect(ui->START,SIGNAL(clicked(bool)),this,SLOT(on_START_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


//定义槽函数
void MainWindow::on_OpenFile()
{
    file_name = QFileDialog::getOpenFileName(NULL,"标题",".","*.jpg");//QFileDialog使用对话框选取本地文件
    qDebug() << file_name;
   // string s =file_name.toStdString();//将读取的QString类型的绝对路径转换为string
    string s =file_name.toLocal8Bit().toStdString();//用于解决中文路径的乱码问题
    src = imread(s,1);//为利用opencv处理图像，这里将初始化Mat型的src
     dst = Mat::zeros( src.rows, src.cols, src.type() );
//    Mat image = cv::imread(file_name.toAscii().data());//报错，toAscii不是QString成员
   // cvNamedWindow("window", CV_WINDOW_NORMAL);
   // imshow("lena秀秀",image);
   //QPixmap image; //定义一张图片,不能设置成全局变量会中断，提示Must construct a QGuiApplication before a QPixmap
       QImage image;
        //image=QImage::QImage(file_name);//加载
        image.load(file_name);//加载,这个居然也可以加载

       //ui->label_src->resize(ui->label_src->pixmap()->size());

        ui->label_src->clear();//清空
        //ui->label_src->resize(QSize(image.width(),image.height()));
        ui->label_src->setFixedSize(500,500);
            //ui->scrollImag->setWidget(ui->label_src);


        ui->label_src->setPixmap(QPixmap::fromImage(image));//加载到Label标签
        ui->label_src->setScaledContents(true);

        ui->label_src->show();//显示


    waitKey(0);
}
void MainWindow::paintEvent(QPaintEvent *event)//重写paintEvent，使用QPainter来绘制背景。
{
    Q_UNUSED(event);

    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::gray);//darkGray\lightGray
    p.drawRect(rect());
}

void MainWindow::on_START_clicked()//begin开始图像处理
{

    switch(key)
    {
    case 0:
        cv::GaussianBlur(src, dst, Size( 5, 5 ), 0, 0 );
         MainWindow::displayResult(dst, ui->label_dst);
        break;
    case 1:
         cv::medianBlur( src, dst, 7);
          MainWindow::displayResult(dst,ui->label_dst);
        break;
    default:
        dst=src.clone();
      MainWindow::displayResult(dst,ui->label_dst);
        break;
    }
//输出处理后的图像
    // MainWindow::displayResult(dst,label_dst);

}

void MainWindow::on_GaussianBlur_clicked()
{
   //key=(enum blur)0;强制类型转换，但是最后还是想用字符串
    key=0;
}



void MainWindow::on_medianBlur_clicked()
{
    //key=(enum blur)1;
    key=1;
}


void MainWindow::displayResult(Mat mat, QLabel *label)
{   //show the resultImage in result area;
    Mat rgb;
    QImage img;
    if(mat.channels() == 3)    // RGB image
    {
        cvtColor(mat,rgb,CV_BGR2RGB);
        img = QImage((const uchar*)(rgb.data),  //(const unsigned char*)
                     rgb.cols,rgb.rows,
                     rgb.cols*rgb.channels(),   //new add
                     QImage::Format_RGB888);
    }else                     // gray image
    {
        img = QImage((const uchar*)(mat.data),
                     mat.cols,mat.rows,
                     mat.cols*mat.channels(),    //new add
                     QImage::Format_Indexed8);
    }
    label->clear();//清空
    //label->resize(QSize(image.width(),image.height()));
    label->setPixmap(QPixmap::fromImage(img));//加载到Label标签
    label->setFixedSize(500,500);
    label->setScaledContents(true);
    label->show();//显示


waitKey(0);
//    resultImageLabel->setPixmap(QPixmap::fromImage(img));
//    resultImageLabel->resize(resultImageLabel->pixmap()->size());
//    resultImageLabel->show();
}

void MainWindow::on_Save_clicked()
{

    //dstpath<<(enum blur)key<<".jpg";
    Qdstpath = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images (*.png *.bmp *.jpg)")); //选择路径
    string dstpath =Qdstpath.toLocal8Bit().toStdString();//加.toLocal8Bit()来支持中文路径
    imwrite(dstpath,dst);
    cv::Mat imag=imread(dstpath);
    if(imag.data)
    {
        QMessageBox::information(this,tr("提示"),tr("保存图像成功!"));
        return;
    }
    else
    {
        QMessageBox::information(this,tr("提示"),tr("保存图像成功!"));
        return;
    }
}

void MainWindow::on_Send_clicked()
{
    QUdpSocket *mysender;
   mysender = new QUdpSocket(this);
  // QImage *imageSend;
   //imageSend->load(Qdstpath);

   QBuffer buffer;
   buffer.open(QIODevice::ReadWrite);
   //pixmap不能为空，必须先将图片加载到pixmap中
   QPixmap pixmap;
   pixmap.load(Qdstpath);
   pixmap.save(&buffer,"jpg");
   QByteArray pixArray;
   pixArray.append(buffer.data());



   //UDP广播
   mysender->writeDatagram(pixArray,QHostAddress::Broadcast,6665);
   //向特定IP发送
   QHostAddress serverAddress = QHostAddress("10.21.11.66");
   mysender->writeDatagram(pixArray,serverAddress, 6665);
}
