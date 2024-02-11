#include "vector3.h"

// sinetric

Vector3::Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {
    
};

float Vector3::GetMagnitude()
{
    return (sqrt(pow(X, 2) + pow(Y, 2) + pow(Z, 2)));
};

Vector3 Vector3::GetUnitVector() {
    float _magn = this->GetMagnitude();

    return Vector3(this->X/_magn, this->Y/_magn, this->Z/_magn);
};