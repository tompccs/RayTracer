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
combined::SetCheck2(bool set){
    check = set;
}

bool
combined::GetCheck2(){
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

void
combined::SetPoint2(Point3D set){
    point2 = set;
}

Point3D
combined::GetPoint2(){
    return point2;
}