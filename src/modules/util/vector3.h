#include <iostream>
#include <math.h>

// sinetric

class Vector3{
    public:
        float X;
        float Y;
        float Z;

        float Magnitude;

        Vector3(float _x, float _y,float _z);

        Vector3 GetUnitVector();
};