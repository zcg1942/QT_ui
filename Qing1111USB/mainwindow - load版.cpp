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

        //int minHessian=700;
        SurfFeatureDetector detector(minHessian);//定义一个SurfFeatureDetector（SURF） 特征检测类对象
        std::vector<KeyPoint> keyPoints1;// keyPoints2;//vector模板类，存放任意类型的动态数组

        //【3】调用detect函数检测出SURF特征关键点，保存在vector容器中
        detector.detect(srcImage1, keyPoints1);
        //detector.detect(srcImage2, keyPoints2);
        drawKeypoints(srcImage1, keyPoints1, dstImage1, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
        //drawKeypoints(srcImage2, keyPoints2, dstImage2, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
        MainWindow::displayResult( dstImage1,ui->label_dst);
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
    Qdstpath = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images (*.png *.bmp *.jpg)")); //选择路径
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

   QBuffer buffer;
   //buffer.open(QIODevice::ReadWrite);
   //pixmap不能为空，必须先将图片加载到pixmap中
   QPixmap pixmap;
   pixmap.load(Qdstpath);
   pixmap.save(&buffer,"jpg");
   QByteArray pixArray;
   pixArray.append(buffer.data());

//本地测试字符串成功，localHost
   QString MyData="hello udpsocket";//要发送的数据
           //mysender->writeDatagram(MyData.toUtf8(),QHostAddress::LocalHost,10000);
           //把数据发送出去,这里的端口你可以随便写，但是要注意它没被占用
   //UDP广播
//   mysender->writeDatagram(pixArray,QHostAddress::Broadcast,6665);
           //本地测试图片，QByteArray,LocalHost
   //mysender->writeDatagram(pixArray.data(),pixArray.size(),QHostAddress::LocalHost,10000);
    mysender->writeDatagram(buffer.data(),QHostAddress::LocalHost,10000);
//   //向特定IP发送
//   QHostAddress serverAddress = QHostAddress("10.21.11.66");
//   mysender->writeDatagram(pixArray,serverAddress, 6665);
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
    QImage qi;//(filename);
         QByteArray zijie;
         QBuffer buffer(&zijie);

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
            return ;
        }

        //端点的使用
        // Find bulk endpoints in the EndPoints[] array
        //EndPoints[i]是端点列表，最大是16，EndPoints[0]指向控制点
        //CCyBulkEndPoint *BulkInEpt = NULL;
        //CCyBulkEndPoint *BulkOutEpt = NULL;

        CCyUSBEndPoint *endpt;//建立一个端点对象
        int eptCount=USBDevice->EndPointCount();//获取所用端点的数目（包括一个控制端点）
        //EndPointCount()虽然是UCHAR类型，可以转化为int型

         CCyUSBEndPoint *BulkOutEpt = USBDevice->EndPointOf(0x02) ;//EndPointOf()函数，该函数直接使用指定的端点，返回其指针
         CCyUSBEndPoint *BulkInEpt = USBDevice->EndPointOf(0x86);
        //读取文件
      QString filename = QFileDialog::getOpenFileName(NULL,"标题",".","*.jpg *.bmp *.png");//QFileDialog使用对话框选取本地文件
       //Qdstpath = QFileDialog::getSaveFileName(this,tr("Save Image"),"",tr("Images (*.png *.bmp *.jpg)"));
       //string s =filename.toLocal8Bit().toStdString();//用于解决中文路径的乱码问题

     qi.load(filename);
      unsigned char * p_bits=qi.bits();

     //int bytecount=qi.byteCount();//读取图像字节数 262144,和buffer大小不一样
      buffer.open(QIODevice::WriteOnly);//只读方式打开缓冲区
    bool save_success=qi.save(&buffer,"bmp");///format=0,会根据后缀猜测类型,但是有问题
//     QByteArray q_all=buffer.data();
       //pixmap.save(&buffer,"jpg");

     if(save_success) //要if判断写入到duffer才能得到size，否则默认是0
     {
          QMessageBox::information(this,"Tips","写入buffer");
        qint64 length=buffer.size();
//        unsigned char  buf=(unsigned char)*(buffer.data());//data()返回的就是指针
//        unsigned char* pbuf=&buf;//不能直接对表达式取&，要引入一个中间变量
//        BulkOutEpt->XferData(pbuf,length);//第一个参数其实是个指针

//         //unsigned char inbuf[length];//不对，要输入常量表达式
//         QByteArray inbuf_B;
//         inbuf_B.resize(length);
//         QBuffer inbuf(&inbuf_B);
//         unsigned char inbuf_u=(unsigned char)*inbuf.data();
//         unsigned char * pinbuf=&inbuf_u;
//         BulkInEpt->XferData(pinbuf,length);//length是随图片大小变化的，默然为0，不能分配大小为0的数组

//         QFile data("E:/qt_code/Qing1111USB/MYData.dat");//文件名通过构造函数传递
//         if(data.open(QIODevice::WriteOnly))//QFile所支持的分隔符是‘/’（斜杠），不支持使用分隔符’\’（反斜杠）。
//         {
//             QDataStream write(&data);
//             //char *writedata=static_cast< char *>(pinbuf);//强制转换数组指针数据类型，有问题
//             //write<<pinbuf;
//             write.writeRawData((char*)pinbuf,sizeof(length));
//             data.flush();
//             if (data.exists())
//             {
//                 QMessageBox::information(this,"Tips","写入成功");
//             }
//         }

         //按照手册中例程的方法
        unsigned char indata[1024]={0};
        unsigned char outdata[1024]={0};//动态分配内存后，分段截取


         //if (myDevice.bHighSpeed)  还不知道怎么判断3.0 还是2.0
           int   XFERSIZE = 1024;
           long xferlen= 1024;
            double sendnum=length/xferlen;//其实这里已经向上取整了
            sendnum=cvCeil(sendnum);//cvCeil()是opencv中向上取整的函数

            bool bEnd=false;//判断是否复制全部图像数据
            bool bResult=true;
            //写二进制文件
            QFile file("E:/qt_code/Qing1111USB/mydata2.dat");//创建QFile文件
             QDataStream out(&file);//实例化
          for(int round=0;(round<sendnum+1)&&!bEnd&bResult;round=round+1)//要加1.round=sendnum时是余数部分
               {
                 QByteArray qoutdata=buffer.data().mid(round*XFERSIZE,XFERSIZE);//mid用于截取其中一段
                  if(round==sendnum)//判断是否相等用==!!!!!!
                  {
                      long last=length % xferlen;//结果是54，是对的

                      for(int i=0;i<last;i++)
                      {
                          outdata[i]=(unsigned char) qoutdata[i];//qbytearray是8位有符号，超过127要注意
                      }
                      bResult= BulkOutEpt->XferData(outdata,last);
                      bResult=BulkInEpt->XferData(indata,last);
                       bool append=file.open(QIODevice::Append);
                         out.writeRawData((const char *)indata,int(last));
                           file.close();
                  }
                  else
                  {
                    for(int i=0;(i<XFERSIZE)&&!bEnd;i++)
                       {
                         if(round*xferlen+i==length-1)//图像（包括余数）的最后一字节也复制完成
                           {
                              bEnd=true;
                           }
                         outdata[i]=(unsigned char)qoutdata[i];
                       }
                    bResult= BulkOutEpt->XferData(outdata,xferlen);
                    bResult=BulkInEpt->XferData(indata,xferlen);




                    //round=round+1;//写到for循环的位置
                    //if (file.open(QIODevice::WriteOnly))
                    bool append=file.open(QIODevice::Append);
                    if (append)//以append模式打开，才能不断添加数据到文件末尾
                     {

                     //file.write(indata);
                     //out<<indata;//QDataStream的流式操作，但是遇到\0会自动结束
                     //out.writeBytes((const char *)&indata,1024);//曲面会自动加Qt特有的标志。对应读取的时候，必须用readBytes。
                        int len1=sizeof(length);//发现二进制dat只有一个字节，调试的时候看这个参数，果然只写入了一个字节长度
                        int len2=int(xferlen);
                     out.writeRawData((const char *)indata,len2);//写入最原始的内存，与readRawData配合使用
//                    if(round=sendnum-1)//round=sendnum-1时是余数部分
//                     {
//                         int last=xferlen*sendnum-length;
//                         char indata_c[1024]={0};
//                       for(int j=0;j<last;j++)
//                        {
//                         indata_c[j]=indata[j];
//                        }
//                        out.writeRawData(indata_c,last);
//                     }

                     //file.Append
                     file.close();
                     //flush刷新，或者close(),这样才能真正写入
                     //QImage myimage;
                     //myimage.loadFromData(out);//读取二进制文件到图像
                     //Mat mydata=imread("mydata.dat");
                     //imshow("接收图像",mydata);

                     //读二进制文件
          //                 QFile readfile("mydata.dat");
          //                 readfile.open(QIODevice::ReadOnly);
          //                 QDataStream in(&readfile);
          //                 //in>>
          //                 in.readRawData()
                     }
                     else
                     {
                         file.errorString();
                     }
                  }
               }


     }




//   for (int i=1; i<eptCount; i++)
//   {
//       //EndPoints[i]是端点列表，最大是16，EndPoints[0]指向控制点
//     bool bIn = ((USBDevice->EndPoints[i]->Address & 0x80)==0x80);//address用来判断传输方向，0x8_是in,0x0_out
//     bool bBulk = (USBDevice->EndPoints[i]->Attributes == 2);//Attritutes用来判断传输类型，2是bulk
//     if (bBulk && bIn)
//       BulkInEpt =(CCyBulkEndPoint *) USBDevice->EndPoints[i];
//      BulkInEpt->TimeOut=1000;// 1 sec timeout
//      BulkInEpt->XferData(inData,length);

//     if (bBulk && !bIn)
//       BulkOutEpt =(CCyBulkEndPoint *) USBDevice->EndPoints[i];
//       BulkOutEpt->TimeOut=1000;// 1 sec timeout
//        BulkOutEpt->XferData(outData,length);

//
//   }



 //FileStream myStream = new FileStream("mydata.dat", FileMode.Append, FileAccess.Write);
//   CCyControlEndPoint *ept = USBDevice->ControlEndPt;
//   //CCyControlEndPoint *ctlEpt = new CCyControlEndPoint(*USBDevice->ControlEndPt);
//   ept->Target = TGT_DEVICE;//一般不必关注，固定为：TGT-DEVICE。
//   ept->ReqType = REQ_VENDOR;//当传输自定义请求时，应该用REQ_VENDOR。
//   ept->Direction = DIR_TO_DEVICE;//表明传输方向为：主机到usb设备
//   ept->ReqCode = 0x05;//ReqCode： 八位二进制码值，不同的码值表示USB芯片应该执行的特定功能和命令
//   ept->Value = 1;//其值由ReqCode决定
//   ept->Index = 0;//其值由ReqCode决定

   //unsigned char buf[512];
   //PUCHAR buf=new UCHAR[buflen];
   //ZeroMemory(buf, 512);//用0填充buf区，填充大小（buflen字节）
   //LONG buflen = 512;
   //ept->XferData(buf, buflen);
   //   LONG bytesToSend = 128;
   //   ept->Write(buf, bytesToSend);//write（）函数首先将Direction参数设置为DIR_TO_DEVICE，然后调用XferData()函数。











}
