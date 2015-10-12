#ifndef __RayTracer2_0__Point3D__
#define __RayTracer2_0__Point3D__

#include <iostream>
#include "Vector3D.h"

class Point3D : public Vector3D
{
public:
    
    Point3D();
    
    Point3D(float r, float s, float t);
    
    Point3D& operator =(const Vector3D& v);
    
    Point3D& operator *=(float t);
    
    Point3D& operator /=(float t);
    
    Point3D& operator &=(const Vector3D& v);
    
    Point3D operator -(void) const;
    
    Point3D operator +(const Vector3D& v) const;
    
    Point3D operator -(const Vector3D& v) const;
    
    Vector3D operator -(const Point3D& p) const;
    
    Point3D operator *(float t) const;
    
    Point3D operator /(float t) const;
    
    Point3D operator &(const Vector3D& v) const;
    
    double distancetopoint(Point3D point);
};

inline Point3D operator *(float t, const Point3D& p)
{
    return (Point3D(t * p.x, t * p.y, t * p.z));
}

struct Origin3D_
{
    const Point3D& operator +(const Vector3D& v)
    {
        return (static_cast<const Point3D&>(v));
    }
    
    Point3D operator -(const Vector3D& v)
    {
        return (Point3D(-v.x, -v.y, -v.z));
    }
};


extern Origin3D_ Origin3D;

#endif
