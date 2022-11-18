#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initializeGUI();
}

MainWindow::~MainWindow()
{}

void MainWindow::initializeGUI()
{
    glWidget = new OpenGLWidget(this);
    glWidget->setGeometry(0, 0, 960, 720);
}

QSize MainWindow::sizeHint() const
{
    return QSize(960, 720);
}
