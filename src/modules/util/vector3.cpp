#include "vector3.h"

// sinetric

Vector3::Vector3(float _x, float _y, float _z): X(_x), Y(_y), Z(_z), Magnitude(sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2))) {
    if (Magnitude != 1.0){
        Vector3 _unitVector(X/Magnitude, Y/Magnitude, Z/Magnitude);

        UnitVector = &_unitVector;
    };
};