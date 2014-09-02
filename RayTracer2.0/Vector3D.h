#ifndef __RayTracer2_0__Vector3D__
#define __RayTracer2_0__Vector3D__

#include <iostream>
#include "Vector2D.h"
#include "Functions.h"

class Vector3D
{
    Functions calc;
public:
    
    double   x;
    double   y;
    double   z;
    
    Vector3D();
    
    Vector3D(double r, double s, double t);
    
    Vector3D(const Vector2D& v, double u);
    
    Vector3D& Set(double r, double s, double t);
    
    Vector3D& Set(const Vector2D& v, double u);
    
    double& operator [](long k);
    
    const double& operator [](long k) const;
    
    Vector3D& operator =(const Vector2D& v);
    
    Vector3D& operator +=(const Vector3D& v);
    
    Vector3D& operator +=(const Vector2D& v);
    
    Vector3D& operator -=(const Vector3D& v);
    
    Vector3D& operator -=(const Vector2D& v);
    
    Vector3D& operator *=(double t);
    
    Vector3D& operator /=(double t);
    
    Vector3D& operator %=(const Vector3D& v);
    
    Vector3D& operator &=(const Vector3D& v);
    
    Vector3D operator -(void) const;
    
    Vector3D operator +(const Vector3D& v) const;
    
    Vector3D operator +(const Vector2D& v) const;
    
    Vector3D operator -(const Vector3D& v) const;
    
    Vector3D operator -(const Vector2D& v) const;
    
    Vector3D operator *(double t) const;
    
    Vector3D operator /(double t) const;
    
    double operator *(const Vector3D& v) const;
    
    double operator *(const Vector2D& v) const;
    
    Vector3D operator %(const Vector3D& v) const;
    
    Vector3D operator &(const Vector3D& v) const;
    
    bool operator ==(const Vector3D& v) const;
    
    bool operator !=(const Vector3D& v) const;
    
    Vector3D& Normalise(void);
    
    Vector3D& RotateAboutX(double angle);
    
    Vector3D& RotateAboutY(double angle);
    
    Vector3D& RotateAboutZ(double angle);
    
    Vector3D& RotateAboutAxis(double angle, const Vector3D& axis);
    
    Vector3D GetRandomUnitVector();

};

inline Vector3D operator *(double t, const Vector3D& v)
{
    return (Vector3D(t * v.x, t * v.y, t * v.z));
}


inline double Dot(const Vector3D& v1, const Vector3D& v2)
{
    return (v1 * v2);
}

inline Vector3D Cross(const Vector3D& v1, const Vector3D& v2)
{
    return (v1 % v2);
}

inline double Magnitude(const Vector3D& v)
{
    return (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}

inline double InverseMag(const Vector3D& v)
{
    return (1.0F / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z));
}

inline double SquaredMag(const Vector3D& v)
{
    return (v.x * v.x + v.y * v.y + v.z * v.z);
}

#endif