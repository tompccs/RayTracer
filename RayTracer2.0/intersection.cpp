#include "Intersection.hpp"

double&
Intersection::GetDistance(){
    return Distance;
}

Vector3D&
Intersection::GetNormal(){
    return Normal;
}

Point3D&
Intersection::GetPoint(){
    return Point;
}