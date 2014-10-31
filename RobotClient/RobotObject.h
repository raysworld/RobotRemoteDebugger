#ifndef ROBOTOBJECT_H
#define ROBOTOBJECT_H

#include "../stdafx.h"

struct FrameInfo
{
    float Joints[9];
    float Xyzwpr[7];
    short Config[9];
    short ut;
    short uf;
};

extern FrameInfo frameInfo;
extern short robotSpeed;
extern short robotMotionStep;

#endif // ROBOTOBJECT_H
