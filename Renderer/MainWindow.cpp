#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(800, 600);

    initializeGUI();
}

MainWindow::~MainWindow()
{}

void MainWindow::initializeGUI()
{
    glWidget = new OpenGLWidget(this);
    glWidget->setGeometry(0, 0, 600, 600);
}
