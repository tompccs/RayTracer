#include "Point3D.h"

//Definitions of a point

Point3D::Point3D()
{}

Point3D::Point3D(float r, float s, float t):
Vector3D(r, s, t)
{}

Point3D&
Point3D::operator =(const Vector3D& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return (*this);
}

Point3D&
Point3D::operator *=(float t)
{
    x *= t;
    y *= t;
    z *= t;
    return (*this);
}

Point3D&
Point3D::operator /=(float t)
{
    float f = 1.0F / t;
    x *= f;
    y *= f;
    z *= f;
    return (*this);
}


Point3D&
Point3D::operator &=(const Vector3D& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return (*this);
}

Point3D
Point3D::operator -(void) const
{
    return (Point3D(-x, -y, -z));
}

Point3D
Point3D::operator +(const Vector3D& v) const
{
    return (Point3D(x + v.x, y + v.y, z + v.z));
}

Point3D
Point3D::operator -(const Vector3D& v) const
{
    return (Point3D(x - v.x, y - v.y, z - v.z));
}

Vector3D
Point3D::operator -(const Point3D& p) const
{
    return (Vector3D(x - p.x, y - p.y, z - p.z));
}

Point3D
Point3D::operator *(float t) const
{
    return (Point3D(x * t, y * t, z * t));
}

Point3D
Point3D::operator /(float t) const
{
    float f = 1.0F / t;
    return (Point3D(x * f, y * f, z * f));
}

Point3D
Point3D::operator &(const Vector3D& v) const
{
    return (Point3D(x * v.x, y * v.y, z * v.z));
}

double // Calculates the difference between two points.
Point3D::distancetopoint(Point3D point){
    double xdif = x - point.x;
    double ydif = y - point.y;
    double zdif = z - point.z;
    
    double distance = sqrt(xdif*xdif + ydif*ydif + zdif*zdif);
    
    return distance;
}


Origin3D_ Origin3D;
