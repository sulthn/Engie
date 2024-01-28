#include "vector3.h"

// sinetric

Vector3::Vector3(float _x, float _y, float _z){
    X = _x;
    Y = _y;
    Z = _z;

    Magnitude = sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2));

    if (Magnitude != 1.0){
        Vector3 UnitVector = this->GetUnitVector();
    };
};

Vector3 Vector3::GetUnitVector() {
    Vector3 UnitVector(X/Magnitude, Y/Magnitude, Z/Magnitude);

    return UnitVector;
}