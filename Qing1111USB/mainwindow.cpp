#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include"stdio.h"
#include <opencv2/opencv.hpp>
#include <QFileDialog>
#include <QDebug>
#include <QString>
#include<QMessageBox>

#include <opencv2/opencv.hpp>
#include <QtNetwork>//qt网络编程

#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/nonfree/nonfree.hpp>
#include "CyAPI.h"//hidapi头文件
#include<opencv2/legacy/legacy.hpp> //BruteForceMatcher实际在该头文件中
using namespace std;
using namespace cv;
#ifdef NULL
#define NULL 0
#endif

//声明全局变量
 //QPixmap image; //设置成全局变量会中断，提示Must construct a QGuiApplication before a QPixmap
  QString file_name;
  Mat srcImage1;
  Mat dst;
  int key=8;
  int minHessian;
  Mat  dstImage1;


 QString Qdstpath;
//  enum blur{GaussianBlur,medianBlur};
//   enum blur key; 还是整型好用


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
   ui->actionOpen_O->setIcon(QIcon(":/Open.png")); // Add code.
   ui->minHessian->setRange(300,1000);
   ui->minHessian->setValue(700);
    connect(ui->actionOpen_O,SIGNAL(triggered()),this,SLOT(on_OpenFile()));//连接信号与槽
     connect(ui->GaussianBlur,SIGNAL(stateChanged(int)),this,SLOT(on_GaussianBlur_clicked()));//连接信号与槽
     connect(ui->Features_dect,SIGNAL(stateChanged(int)),this,SLOT(on_Features_dect_clicked()));
     connect(ui->medianBlur,SIGNAL(stateChanged(int)),this,SLOT(on_medianBlur_clicked()));//连接信号与槽
     connect(ui->START,SIGNAL(clicked(bool)),this,SLOT(on_START_clicked()));
     connect(ui->Send,SIGNAL(clicked(bool)),this,SLOT(on_Send_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


//定义槽函数
void MainWindow::on_OpenFile()
{
    file_name = QFileDialog::getOpenFileName(NULL,"标题",".","*.bmp *.jpg *.png");//QFileDialog使用对话框选取本地文件
     //QString path = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.jpg *.png)"));
    qDebug() << file_name;
   // string s =file_name.toStdString();//将读取的QString类型的绝对路径转换为string
    string s =file_name.toLocal8Bit().toStdString();//用于解决中文路径的乱码问题
   srcImage1 = imread(s,1);//为利用opencv处理图像，这里将初始化Mat型的src
     dst = Mat::zeros( srcImage1.rows, srcImage1.cols,srcImage1.type() );
      dstImage1=srcImage1.clone();
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
    {
        cv::GaussianBlur(srcImage1, dstImage1, Size( 5, 5 ), 0, 0 );
         MainWindow::displayResult(dstImage1, ui->label_dst);
    }
        break;
    case 1:
    {
        cv::medianBlur(srcImage1, dstImage1, 7);
          MainWindow::displayResult(dstImage1,ui->label_dst);
    }
        break;
    case 2:
    {
        //获得参考图像
        file_name = QFileDialog::getOpenFileName(NULL,"标题",".","*.bmp *.jpg *.png");//QFileDialog使用对话框选取本地文件
         //QString path = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files(*.jpg *.png)"));
        qDebug() << file_name;
       // string s =file_name.toStdString();//将读取的QString类型的绝对路径转换为string
        string s =file_name.toLocal8Bit().toStdString();//用于解决中文路径的乱码问题
          cv::Mat srcImage2 = imread(s,1);//为利用opencv处理图像，这里将初始化Mat型的src
            Mat dstImage2=srcImage2.clone();

        //int minHessian=700;
        SurfFeatureDetector detector(minHessian);//定义一个SurfFeatureDetector（SURF） 特征检测类对象
        std::vector<KeyPoint> keyPoints1, keyPoints2;//vector模板类，存放任意类型的动态数组

        //【3】调用detect函数检测出SURF特征关键点，保存在vector容器中
        detector.detect(srcImage1, keyPoints1);
        detector.detect(srcImage2, keyPoints2);

        drawKeypoints(srcImage1, keyPoints1, dstImage1, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
        drawKeypoints(srcImage2, keyPoints2, dstImage2, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
        MainWindow::displayResult( dstImage1,ui->label_dst);



         //Mat dst = Mat::zeros( srcImage2.rows, srcImage2.cols,srcImage2.type() );

        //【4】计算描述符（特征向量）
            SurfDescriptorExtractor extractor;
            Mat descriptors1, descriptors2;
            extractor.compute(srcImage1, keyPoints1, descriptors1);
            extractor.compute(srcImage2, keyPoints2, descriptors2);

            //【5】使用BruteForce进行匹配
            // 实例化一个匹配器
            BruteForceMatcher< L2<float> > matcher;
            std::vector< DMatch > matches;
            //匹配两幅图中的描述子（descriptors）
            matcher.match(descriptors1, descriptors2, matches);

            //【6】绘制从两个图像中匹配出的关键点
                Mat imgMatches;
                drawMatches(srcImage1, keyPoints1, srcImage2, keyPoints2, matches, imgMatches);//进行绘制
                qDebug()<< "number of matched points: " << matches.size() << endl;//<< matches.size()

                //【7】显示匹配效果图
                namedWindow("Matches");
                cv::imshow("Matches",imgMatches);
                //【8】特征点匹配对排序
                sort(matches.begin(), matches.end()); //特征点排序
                //获取排在前N个的最优匹配特征点
                vector<Point2f> imagePoints1, imagePoints2;
                for (int i = 0; i<3; i++)
                {
                    imagePoints1.push_back(keyPoints1[matches[i].queryIdx].pt);
                    imagePoints2.push_back(keyPoints2[matches[i].trainIdx].pt);
                }
                //【9】求得仿射变换
                Mat warpMat(2, 3, CV_32FC1);
                warpMat = getAffineTransform(imagePoints1, imagePoints2);

                //【10】对源图像应用刚刚求得的仿射变换
                Mat  dstImage_warp;
                warpAffine(srcImage1, dstImage_warp, warpMat, dstImage_warp.size());
                imshow("After warp", dstImage_warp);


    }
        break;
    default:
        dstImage1=srcImage1.clone();
      MainWindow::displayResult(dstImage1,ui->label_dst);
        break;
    }
}
//输出处理后的图像
    // MainWindow::displayResult(dst,label_dst);



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

void MainWindow::on_Features_dect_clicked()
{
    key=2;
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
    Qdstpath = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images ( *.bmp)")); //选择路径
    string dstpath =Qdstpath.toLocal8Bit().toStdString();//加.toLocal8Bit()来支持中文路径
    imwrite(dstpath,dstImage1);
    cv::Mat imag=imread(dstpath);
    if(imag.data)
    {
        QMessageBox::information(this,tr("提示"),tr("保存图像成功!"));
        return;
    }
    else
    {
        QMessageBox::information(this,tr("提示"),tr("保存图像失败!"));
        return;
    }
}

void MainWindow::on_Send_clicked()
{
    QUdpSocket *mysender;
   mysender = new QUdpSocket(this);
  // QImage *imageSend;
   //imageSend->load(Qdstpath);

   //load方法

   QBuffer buffer;
   //buffer.open(QIODevice::ReadWrite);
   //pixmap不能为空，必须先将图片加载到pixmap中
   QPixmap pixmap;
   pixmap.load(Qdstpath);
   pixmap.save(&buffer,"jpg");
   QByteArray pixArray;
   pixArray.append(buffer.data());
    mysender->writeDatagram(buffer.data(),QHostAddress::LocalHost,10000);

//本地测试字符串成功，localHost
   //QString MyData="hello udpsocket";//要发送的数据
           //mysender->writeDatagram(MyData.toUtf8(),QHostAddress::LocalHost,10000);
           //把数据发送出去,这里的端口你可以随便写，但是要注意它没被占用
   //UDP广播
//   mysender->writeDatagram(pixArray,QHostAddress::Broadcast,6665);
           //本地测试图片，QByteArray,LocalHost
   //mysender->writeDatagram(pixArray.data(),pixArray.size(),QHostAddress::LocalHost,10000);

//   //向特定IP发送
//   QHostAddress serverAddress = QHostAddress("10.21.11.66");
//   mysender->writeDatagram(pixArray,serverAddress, 6665);

    //用发送文件的方法发送图片

          /*qDebug()<<Qdstpath<<endl;
          if(Qdstpath.isEmpty()){
              return;
          }
          //localaddr1.setAddress("192.168.0.101");
          QFile file;
              file.setFileName(Qdstpath);
              if(!file.open(QIODevice::ReadOnly)) return;
              QByteArray begin ="Begin!";
               mysender->writeDatagram(begin,QHostAddress::LocalHost,10000);
              while(!file.atEnd())
              {
                  QByteArray line=file.read(8000);
                  mysender->writeDatagram(line,QHostAddress::LocalHost,10000);
                  _sleep(50);//延迟50ms

                  if(line.size()<8000)
                   {
                      QByteArray str = "End!";
                      mysender->writeDatagram(str.data(),str.size(),QHostAddress::LocalHost,10000);//发送,表明发送结束

                   }

              }*/
}

void MainWindow::readData()
{
    while (read->hasPendingDatagrams())
    {
        QByteArray MyDate;//因为传送来的数据类型是未知，所以用bytearray
        MyDate.resize(read->pendingDatagramSize());//mydata的数据大小取决于接收到的数据
        read->readDatagram(MyDate.data(),MyDate.size());//读取数据

        qDebug()<<QString::fromUtf8(MyDate);
        QMessageBox::information(this,tr("接收"),tr("要显示该图片吗？"),QMessageBox::Yes|QMessageBox::No);
        // 选择是

            QPixmap imageresult;//接收的QByteArray转化为Qpixmap
            //imageresult.loadFromData(MyDate);//接收的QByteArray转化为Qpixmap
             imageresult.load("b.jpg");
            // 创建一个对话框对象
             QDialog * dlg = new QDialog;
            // 设置对话框标题文字
            dlg->setWindowTitle("title");
             //添加QLabel控件
           receivedlabel = new QLabel;
             //QLabel *label = new QLabel;  //创建QLabel控件
             //setCentralWidget(label);  //将当前控件至于中央
             //label->setText(tr("Hello World!"));  //设置控件文本
            receivedlabel->setPixmap(imageresult);

             //label->setAlignment(Qt::AlignCenter);  //文本水平和垂直都居中
            receivedlabel->setFixedSize(500,500);
            receivedlabel->setScaledContents(true);
            receivedlabel->show();//显示

    }
}



void MainWindow::on_minHessian_editingFinished()
{

}

void MainWindow::on_minHessian_valueChanged(int arg1)
{
     minHessian=ui->minHessian->value();
}

void MainWindow::on_USEUSB_clicked()
{
     // byte[] outData = new byte[1024];byte是c#的关键字，c++中没有
    //定义传输的数组和长度，if语句中好像不能定义
      //unsigned char *outData=new unsigned char[1024];动态创建，但是不知道怎么赋值，而且有局限性
      //unsigned char buf[] = "hello world";


//    unsigned char outData[] = {"hello world"};
   // LONG length = 11;
//      unsigned char inData[11];


//         string deviceName= USBDevice->DeviceName;
//         ushort VID= USBDevice->VendorID;
//          ushort PID= USBDevice->ProductID;

        CCyUSBDevice *USBDevice= new CCyUSBDevice(NULL);
//        USBDevice->Open(0);//打开0号设备
//        if (! USBDevice->Open(0))
//        {
//           QMessageBox::information(this,"Tips","USB doesn't connect");

//        }
        int devices=USBDevice->DeviceCount();
        int d=0;
        int vID,pID;
        //如果有多个USB设备，用枚举的方法
        if (devices)//保证至少有一个USB设备连接
        {
//            for (int i=0;i<USBDevice->DeviceCount();i++)
//            {
//                USBDevice->Open(i);//依次打开第i号设备
//                //m_DeviceListComBox.add
//            }
            do {
            USBDevice->Open(d); // Open automatically calls Close() if necessary
            vID = USBDevice->VendorID;
            pID = USBDevice->ProductID;
            d++;
            } while ((d < devices ) && (vID != 0x04B4) && (pID !=0x00F0));

        }
        else
        {
            QMessageBox::information(this,"Tips","USB doesn't connect");
            //return ;
        }



        CCyUSBEndPoint *endpt;//建立一个端点对象
        int eptCount=USBDevice->EndPointCount();//获取所用端点的数目（包括一个控制端点）
        //EndPointCount()虽然是UCHAR类型，可以转化为int型

         CCyUSBEndPoint *BulkOutEpt = USBDevice->EndPointOf(0x02) ;//EndPointOf()函数，该函数直接使用指定的端点，返回其指针
         CCyUSBEndPoint *BulkInEpt = USBDevice->EndPointOf(0x86);
        //读取文件
      QString filename = QFileDialog::getOpenFileName(NULL,"标题",".","*.jpg *.bmp *.png");//QFileDialog使用对话框选取本地文件
       //Qdstpath = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images (*.png *.bmp *.jpg)"));
       //string s =filename.toLocal8Bit().toStdString();//用于解决中文路径的乱码问题

      QImage qi;//(filename);
      qi.load(filename);
      int qicount=qi.byteCount();
           QByteArray zijie;
           QBuffer buffer(&zijie);

      //文件方式传输图像
                       QFile ofile;
                       ofile.setFileName(filename);
                       qint64 ofilesize=ofile.size();//263222字节
                       //double oxferlen=512.0;//其中一个除数改为double型可以得到精确小数
                       qint64 oxferlen=512;
                       int oNum=ofilesize/oxferlen;//即便是double型，oNum也是四舍五入后的514.0,
                       //qint64 oNum_ceil=cvCeil(oNum);//整数相除，默认整数，所以这里的向上取整没有意义
                       long rest=ofilesize % oxferlen;
                       int flag=0;


                       if(!ofile.open(QIODevice::ReadOnly)) return;
                       QFile received("E:/odata.dat");
                       received.open(QIODevice::Truncate);//希望每次执行清除之前文件夹的内容

                       while(!ofile.atEnd())
                       {

                            QByteArray line=ofile.read(512);//每次读512个字节
                           //mysender->writeDatagram(line,QHostAddress::LocalHost,10000);
                           unsigned char odata[512]={0};

                           //unsigned char* odata=(unsigned char)line;
                            long olen=line.size();


                            if (flag==oNum)
                            {
                                for(int i=0;i<rest;i++)
                               {
                                  odata[i]=(unsigned char) line[i];
                               }
                                BulkOutEpt->XferData(odata,rest);
                              unsigned char idata[512]={0};//这里长度是512也没关系，只要接收时知道哪里停止
                                BulkInEpt->XferData(idata,rest);

                                QDataStream write(&received);
                                  if(received.open(QIODevice::Append))
                                {
                                 write.writeRawData((const char *)idata,rest);
                                 received.close();
                                }
                            }
                            else
                            {

                               for(int i=0;i<512;i++)
                              {
                                 odata[i]=(unsigned char) line[i];
                              }
                               BulkOutEpt->XferData(odata,olen);
                             unsigned char idata[512]={0};
                               BulkInEpt->XferData(idata,olen);

                               QDataStream write(&received);
                                 if(received.open(QIODevice::Append))
                               {
                                write.writeRawData((const char *)idata,512);
                                received.close();
                               }
                            }


                            flag++;



                       }
                        ofile.reset();

}
