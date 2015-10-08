#include "combined.hpp"

combined::combined(){}

void
combined::SetCheck(bool set){
    check = set;
}

bool
combined::GetCheck(){
    return check;
}

void
combined::SetPoint(Point3D set){
    point = set;
}

Point3D
combined::GetPoint(){
    return point;
}