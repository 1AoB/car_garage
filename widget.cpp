#include "widget.h"
#include "ui_widget.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>







Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //实时时间
    timer_ = new QTimer(this);
    timer_->start(1000);
    connect(timer_,&QTimer::timeout,this,&Widget::SltUpdateTime);


    //打开 摄像头
    QCameraInfo info = QCameraInfo::defaultCamera();
    qDebug() << info.description();//设备名称,型号,厂商
    qDebug() << info.deviceName();//摄像头的名称,是由系统自动生成的一串唯一的标识符
    //代表摄像头
    camera = new QCamera(info);
    //捕获摄像头画面
    view = new QCameraViewfinder(ui->label);
    //给捕获器设置位置,大小
    view->move(0,0);
    view->resize(ui->label->size());
    qDebug()<<ui->label->size().rwidth()<<","<<ui->label->size().rheight();
    //给摄像头设置画面捕捉器
    camera->setViewfinder(view);

    //截图
    cap = new QCameraImageCapture(camera);
    bool flag = connect(cap,&QCameraImageCapture::imageCaptured,this,&Widget::save_image);
    if(!flag)
    {
        QMessageBox::information(this,"提示","信号与槽连接失败!");
    }else{
        QMessageBox::information(this,"提示","信号与槽连接成功!");
    }


    //识别
    //实例化
    client = new QNetworkAccessManager(this);
    //等待百度服务器给我们响应,去读取响应到的数据
    connect(client,
            &QNetworkAccessManager::finished,
            this,
            &Widget::read_reply);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{

}


//保存捕获的图片
void Widget::save_image(int id, QImage img)//QImage img表示捕获的图片
{
    qDebug()<<"正在截图...";
    //将文件保存到debug文件夹下的img文件夹下
    QString now = "F:\\QT_code\\1study\\flushbonading_Curriculum_design\\car_garage\\img.jpg";
    if(!now.isNull())
    {
        img.save(now);/////////////QImage img保存到now的路径////////////////////////
        QMessageBox::information(this,"提示","截图成功");
    }else{
        QMessageBox::information(this,"提示","截图失败");
    }

}

void Widget::on_openBtn_clicked()
{
    qDebug()<<"正在打开摄像头...";

    //先将label清空
    ui->label->clear();
    //摄像头亮
    this->camera->start();
    view->show();//显示捕获画面
}

//截图
void Widget::on_cutBtn_clicked()
{
    cap->capture();
}
//打开已截到的图
void Widget::on_openbtn_clicked()
{
    this->camera->stop(); // 先暂停摄像头
    this->view->hide();   //将QCameraViewfinder暂时隐藏
    //delete view ;

    //获取打开图片的路径
    imgPath = "F:\\QT_code\\1study\\flushbonading_Curriculum_design\\car_garage\\img.jpg";

    QPixmap pix(imgPath);
    ui->label->setScaledContents(true);
    ui->label->setPixmap(pix);


    //将图片设置到picture_label中,并获得图片的数据QByteArray imageData;
    QFile imageFile(imgPath);
    if (!imageFile.open(QFile::ReadOnly)) {//只读方式读取文件内容 在QT中,不是Q开头的,基本都是枚举值
        QMessageBox::information(this,"提示","Failed to open image file:" + imgPath);
    }


    imageData = imageFile.readAll().toBase64().toPercentEncoding();

    //qDebug()<<imgPath;
    //qDebug()<<imageData;

    imageFile.close();
}

//开始识别
void Widget::on_beginBtn_clicked()
{
    qDebug()<<"识别ing...";

    QString url =
            "https://aip.baidubce.com/rest/2.0/ocr/v1/license_plate?access_token="+access_token;
    //图像文件的准备
    //imageData

    //请求准备
    QUrlQuery parame;
    parame.addQueryItem("image",imageData);
    //parame.addQueryItem("multi_detect","false");
    //parame.addQueryItem("multi_scale","false");

    QNetworkRequest request(QUrl(url).toString());
    //给请求设置头部信息
    //如使用 POST 方式，Content-Type 请指定为：application/x-www-form-urlencoded
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    client->post(request,parame.toString(QUrl::FullyEncoded).toUtf8());

}

//读取应答包
void Widget::read_reply(QNetworkReply *reply)
{
    //读取应答
    //QString content = reply->readAll();
    //qDebug()<<"connect:"<<content;
    //qDebug()<<"============================";


    QJsonObject json =  QJsonDocument::fromJson(reply->readAll()).object();

    if(json.contains("words_result"))
    {
       //json.value("words_result")与json["words_result"]等价
       QJsonObject data = json["words_result"].toObject();//读取key是trans_result的QJsonArray数组

       QString ans1 = data["color"].toString();//车牌颜色
       QString ans2 = data["number"].toString();//车牌号码
       qDebug()<<ans1;
       qDebug()<<ans2;

       ui->textEdit->append("车牌颜色:"+ans1);
       ui->textEdit->append("车牌号码:"+ans2);

    }else if(json.contains("error_code")){
        QMessageBox::information(this,"提示","翻译失败!");
        qDebug()<<"出错了";
    }else{
        qDebug()<<"未知错误";
    }
}

void Widget::SltUpdateTime(){

    QDateTime now = QDateTime::currentDateTime();
    QString time_str = now.toString("yyyy-MM-dd hh:mm:ss");

    ui->lcd->display(time_str);
}

