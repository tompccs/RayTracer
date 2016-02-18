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
    return check2;
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

void
combined::SetDistance1(double d){
    distance1 = d;
}

void
combined::SetDistance2(double d){
    distance2 = d;
}

double&
combined::GetDistance1(){
    return distance1;
}

double&
combined::GetDistance2(){
    return distance2;
}