#include "widget.h"

#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    w.setFixedSize(758,660);

    //设计样式
    QFile file("F:\\QT_code\\1study\\flushbonading_Curriculum_design\\car_garage\\style.qss");
    file.open(QFile::ReadOnly);
    QByteArray arr = file.readAll();
    a.setStyleSheet(arr);//a指的是整个应用程序,给a设置样式就是给全部设置
    file.close();

    // 设置项目名称和图标
    w.setWindowTitle("QT实训结课项目-车库拍照识别");
    w.setWindowIcon(QIcon(QPixmap("C:\\Users\\wxn\\Desktop\\2.jpg")));

    w.show();
    return a.exec();
}
