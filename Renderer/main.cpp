#include "MainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    //禁用高分屏下窗体自动缩放，避免OpenGL窗口缩放导致出现图像与窗体大小不一致
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Floor);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
