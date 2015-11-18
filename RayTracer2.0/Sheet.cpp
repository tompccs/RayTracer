#include "Sheet.h"

Sheet::Sheet(): //Constructor
A(),B(),C(),normal()
{}

//Constructor
Sheet::Sheet(const Point3D& a, const Point3D& b, const Point3D& c):
A(a),B(b),C(c),normal()
{}

Sheet::Sheet(const Sheet& sheet): //Constructor
A(sheet.A),B(sheet.B),C(sheet.C),normal(sheet.normal)
{}

Sheet::~Sheet() //Destructor
{}

Sheet& //Set
Sheet::Set(const Point3D &a, const Point3D &b, const Point3D &c){
    A = a;
    B = b;
    C = c;
    return (*this);
}

Sheet& //Return Sheet
Sheet::Get(){
    return (*this);
}

//Sets
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

//Returns
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

//Lengths of sides
double
Sheet::GetABLength(){
    return Magnitude(B-A);
}

double
Sheet::GetACLength(){
    return Magnitude(C-A);
}

//Area
double
Sheet::GetArea(){
    return GetABLength()*GetACLength();
}

//Get Normal vector
Vector3D
Sheet::GetNormal(){
    if(obtainednormal==0){
        normal = Cross((B-A),(C-A)).Normalise();
        obtainednormal = 1;
    }
    
    return normal;
}

//Overrides Normal vector
Vector3D
Sheet::OverrideNormal(){
    normal = Cross((B-A),(C-A)).Normalise();
    obtainednormal = 1;
    
    return normal;
}

//Checks if photon will intersect plane
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

Point3D //Returns point of intersection
Sheet::IntersectionPoint(Photon *photon){
    double a = GetNormal()*GetA();
    double b = GetNormal()*photon->GetPosition();
    double c = GetNormal()*photon->GetMomentum();
    double t = (a-b)/c;
    return (photon->GetPosition() + photon->GetMomentum()*t);
}

double //Returns distance until intersection
Sheet::IntersectionDistance(Photon *photon){
    double t = INFINITY;
    double a = GetNormal()*GetA();
    double b = GetNormal()*photon->GetPosition();
    double c = GetNormal()*photon->GetMomentum();
    
    double value = (a-b)/c;
    
    if(value>1e-5){
        t = value;
    }
    
    return t;
}

bool //Returns if a point on a plane, is on a sheet
Sheet::PointonSheetTest(const Point3D &p){
    Vector3D AB = Cross(p-GetA(),p-GetB()).Normalise();
    Vector3D BD = Cross(p-GetB(),p-GetD()).Normalise();
    Vector3D DC = Cross(p-GetD(),p-GetC()).Normalise();
    Vector3D CA = Cross(p-GetC(),p-GetA()).Normalise();
    
    if (AB == BD && BD == DC && DC == CA) return true;
    return false;
    
}

bool //Checks if photon will intersect sheet
Sheet::GetIntersectionTest(Photon *photon){
    if(PhotonPlaneCheck(photon)&PointSheetCheck(LinePlaneIntersectPoint(photon))) return 1;
    return 0;
}

bool
Sheet::operator==(const Sheet &s){
    if( A == s.A && B == s.B && C == s.C) return 1;
    return 0;
}


Point3D //Returns point of intersection
Sheet::LinePlaneIntersectPoint(Photon *photon){
    return photon->GetPosition() + (photon->GetMomentum() * IntersectionDistance(photon));
}

bool //Checks if a point is on a sheet
Sheet::PointSheetCheck(Point3D point){
    Vector3D AB = Cross(point-GetA(),point-GetB());
    Vector3D BD = Cross(point-GetB(),point-GetD());
    Vector3D DC = Cross(point-GetD(),point-GetC());
    Vector3D CA = Cross(point-GetC(),point-GetA());
    
    if(SignCompare(Dot(AB,BD), Dot(BD,DC), Dot(DC,CA))) return 1;
    
    
    return 0;
}

bool //Checks if 3 numbers are all positive
Sheet::SignCompare(double A, double B, double C){
    if((A>=0 && B>=0 && C>=0) || (A<0 && B<0 && C<0)) return 1;
    return 0;
}

Point3D //Returns centre point of a sheet
Sheet::Centre(){
    return A + ((B-A)/2) + ((C-A)/2);
}
