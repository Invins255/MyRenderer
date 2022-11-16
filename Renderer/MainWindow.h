#pragma once

#include <QtWidgets/QMainWindow>
#include "openglwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    OpenGLWidget* glWidget;

private:

    void initializeGUI();

};
