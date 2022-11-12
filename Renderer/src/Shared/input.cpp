#include "input.h"

void Input::reset()
{
    isMouseMoving = false;
    wheelAngleDelta = QPoint(0.0f, 0.0f);
}

Input::Input()
{
    for(int i=0; i<1024; i++){
        keys[i] = false;
    }
}
