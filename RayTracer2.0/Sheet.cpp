#include "Sheet.h"

Sheet::Sheet():
A(),B(),C(),normal()
{}

Sheet::Sheet(const Point3D& a, const Point3D& b, const Point3D& c):
A(a),B(b),C(c),normal()
{}

Sheet::Sheet(const Sheet& sheet):
A(sheet.A),B(sheet.B),C(sheet.C),normal(sheet.normal)
{}

Sheet::~Sheet()
{}

Sheet&
Sheet::Set(const Point3D &a, const Point3D &b, const Point3D &c){
    A = a;
    B = b;
    C = c;
    return (*this);
}

Sheet&
Sheet::Get(){
    return (*this);
}

void
Sheet::SetA(const Point3D &a){
    A = a;
}

void
Sheet::SetB(const Point3D &b){
    A = b;
}

void
Sheet::SetC(const Point3D &c){
    A = c;
}

Point3D&
Sheet::GetA(){
    return A;
}

Point3D&
Sheet::GetB(){
    return B;
}

Point3D&
Sheet::GetC(){
    return C;
}

Point3D
Sheet::GetD(){
    return C + (B-A);
}

double
Sheet::GetABLength(){
    return Magnitude(B-A);
}

double
Sheet::GetACLength(){
    return Magnitude(C-A);
}

double
Sheet::GetArea(){
    return GetABLength()*GetACLength();
}

Vector3D
Sheet::GetNormal(){
    if(obtainednormal==0){
        normal = Cross((B-A),(C-A)).Normalise();
        obtainednormal = 1;
    }
    
    return normal;
}

bool
Sheet::PhotonPlaneCheck(Photon *photon){
    double a = GetNormal()*GetA();
    double b = GetNormal()*photon->GetPosition();
    double c = GetNormal()*photon->GetMomentum();
    double t = (a-b)/c;
    if(c==0 || t<=0){
        return false;
    }
    else{
        return true;
    }
}

Point3D
Sheet::IntersectionPoint(Photon *photon){
    double a = GetNormal()*GetA();
    double b = GetNormal()*photon->GetPosition();
    double c = GetNormal()*photon->GetMomentum();
    double t = (a-b)/c;
    return (photon->GetPosition() + photon->GetMomentum()*t);
}

double
Sheet::IntersectionDistance(Photon *photon){
    double a = GetNormal()*GetA();
    double b = GetNormal()*photon->GetPosition();
    double c = GetNormal()*photon->GetMomentum();
    double t = (a-b)/c;
    return t;
}

bool
Sheet::PointonSheetTest(const Point3D &p){
    Vector3D AB = Cross(p-GetA(),p-GetB()).Normalise();
    Vector3D BD = Cross(p-GetB(),p-GetD()).Normalise();
    Vector3D DC = Cross(p-GetD(),p-GetC()).Normalise();
    Vector3D CA = Cross(p-GetC(),p-GetA()).Normalise();
    
    if (AB == BD && BD == DC && DC == CA) return true;
    return false;

}

bool
Sheet::GetIntersectionTest(Photon *photon){
    if(PhotonPlaneCheck(photon)&PointSheetCheck(LinePlaneIntersectPoint(photon))) return 1;
    return 0;
}

bool
Sheet::operator==(const Sheet &s){
    if( A == s.A && B == s.B && C == s.C) return 1;
    return 0;
}


Point3D
Sheet::LinePlaneIntersectPoint(Photon *photon){
    return photon->GetPosition() + (photon->GetMomentum() * IntersectionDistance(photon));
}

bool
Sheet::PointSheetCheck(Point3D point){
    Vector3D AB = Cross(point-GetA(),point-GetB());
    Vector3D BD = Cross(point-GetB(),point-GetD());
    Vector3D DC = Cross(point-GetD(),point-GetC());
    Vector3D CA = Cross(point-GetC(),point-GetA());
    
    if(SignCompare(Dot(AB,BD), Dot(BD,DC), Dot(DC,CA))) return 1;

    
    return 0;
}

bool
Sheet::SignCompare(double A, double B, double C){
    if((A>=0 && B>=0 && C>=0) || (A<0 && B<0 && C<0)) return 1;
    return 0;
}

Point3D
Sheet::Centre(){
    return A + ((B-A)/2) + ((C-A)/2);
}
