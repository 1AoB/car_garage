#ifndef WIDGET_H
#define WIDGET_H

#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraViewfinder>
#include <QWidget>

#include <QCameraInfo>
#include <QMessageBox>
#include <QNetworkAccessManager>


#include <QDebug>

#include <QNetworkRequest>//请求
#include <QNetworkReply>//回复
#include <QUrlQuery>

#include <QCryptographicHash>
#include <QDateTime>

#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    //实时时间
    void SltUpdateTime();

    void on_pushButton_clicked();

    //截图
    void save_image(int id, QImage img);

    void on_openBtn_clicked();

    void on_cutBtn_clicked();

    void on_openbtn_clicked();

    void on_beginBtn_clicked();

    //识别
    void read_reply(QNetworkReply * reply);

private:
    Ui::Widget *ui;

    //实时时间
    QTimer *timer_;
private:
    //打开摄像头
    QCamera *camera;//摄像头
    QCameraViewfinder *view;//捕捉摄像头并显示
    QCameraImageCapture *cap;//截图


    QNetworkAccessManager* client;
    QString apiUrl = "https://aip.baidubce.com/rest/2.0/ocr/v1/license_plate";
    QString appId = "32714526";
    QString apiKey = "pZkwfkgnDUAa5Ag69LoSN1oe";
    QString secretKey = "4PKHvy7ZKSTF3mYPcj8CvXyXZ7v03QE0";
    QString imgPath;

    //老师的
    QString access_token =  "24.c31e07fdc386328128bb9eedf7ac4c7d.2592000.1684804184.282335-24592129";

    //我的
    //QString access_token = "24.06ca26e29d450a04a12154771294fbf1.2592000.1684747860.282335-32714526";

    QByteArray imageData;
};
#endif // WIDGET_H
