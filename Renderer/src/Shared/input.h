#ifndef INPUT_H
#define INPUT_H

#include <QPointF>

class Input
{
public:
    static Input& getInstance(){
        static Input instance;
        return instance;
    }
    //keyboard
    bool keys[1024];
    //mouse
    bool leftButtonPressed;
    bool middleButtonPressed;
    bool rightButtonPressed;
    bool isMouseMoving;
    QPointF lastMousePos;
    QPointF currentMousePos;
    QPoint wheelAngleDelta;

    void reset();

private:
    Input();
    Input(Input&) = delete;
    Input& operator=(Input&) = delete;
};

#endif // INPUT_H
