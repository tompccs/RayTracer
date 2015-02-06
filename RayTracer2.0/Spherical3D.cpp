#include "Spherical3D.h"

Spherical3D::Spherical3D(){}

Spherical3D::Spherical3D(double r, double theta, double phi){
    this->r = r;
    this->theta = theta;
    this->phi = phi;
}

Spherical3D::Spherical3D(Vector3D& cartesian){
    
    r = sqrt(cartesian.x*cartesian.x + cartesian.y*cartesian.y + cartesian.z*cartesian.z);
    
    theta = acos(cartesian.y/this->r);
    
    phi = atan2(cartesian.y, cartesian.x);
    
}

Vector3D
Spherical3D::Cartesian(){
    
    Vector3D c;
    
    c.x = r * sin(theta) * cos(phi);
    c.y = r * sin(phi) * sin(theta);
    c.z = r * cos(theta);
    
    return c;
}

Spherical3D&
Spherical3D::Get(){
    return (*this);
}

void
Spherical3D::Print(){
    cout<<"Spherical Coordinates:"<<endl;
    cout<<"r = "<<r<<endl;
    cout<<"theta = "<<theta<<endl;
    cout<<"phi = "<<phi<<endl;
    cout<<endl;
}

