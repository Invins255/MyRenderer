#include "MainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    //���ø߷����´����Զ����ţ�����OpenGL�������ŵ��³���ͼ���봰���С��һ��
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Floor);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
