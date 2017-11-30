#-------------------------------------------------
#
# Project created by QtCreator 2017-10-26T09:08:20
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qing1026
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h \
    CyUSB30_def.h
#HEADERS += hidapi.h //添加第三方hidapi库.-L指定当前的路径；_PRO_FILE_PWD_指当前.pro文件所在的目录；-l指定.dll文件
    #LIBS += D:\XMPCache\Qt\Qing1101\hidapi.lib\ //这里添加的是绝对路径，按照网上的反而错了
HEADERS += CyAPI.h
LIBS += E:\qt_code\Qing1111USB\CyAPI.lib


FORMS += \
        mainwindow.ui

#添加OPenCV库
INCLUDEPATH+=D:\program\opencv\build\include\
INCLUDEPATH+=D:\program\opencv\build\include\opencv\
INCLUDEPATH+=D:\program\opencv\build\include\opencv2

CONFIG(debug,debug|release): {
LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_calib3d2413d.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_contrib2413d.lib

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_core2413d.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_features2d2413d.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_flann2413d.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_gpu2413d.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_highgui2413d.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_imgproc2413d.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_legacy2413d.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_ml2413d.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_nonfree2413d.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_objdetect2413d.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_ocl2413d.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_photo2413d.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_stitching2413d.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_superres2413d.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_ts2413d.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_video2413d.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_videostab2413d.lib

}
 else:CONFIG(release, debug|release):
{

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_calib3d2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_core2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_flann2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_gpu2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_highgui2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_imgproc2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_features2d2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_ml2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_nonfree2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_objdetect2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_ocl2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_photo2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_legacy2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_stitching2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_superres2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_ts2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_video2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_videostab2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_videostab2413.lib\

LIBS+=D:\program\opencv\build\x64\vc12\lib\opencv_contrib2413.lib


}

RESOURCES += \
    icon.qrc

DISTFILES += \
    CyAPI.lib

