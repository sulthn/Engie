#pragma once

#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include <math.h>

// sinetric

struct Vector3 {
    float X, Y, Z;

    Vector3(float x, float y, float z);

    float GetMagnitude();

    Vector3 GetUnitVector();
};

#endif