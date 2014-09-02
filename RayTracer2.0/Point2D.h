#ifndef __RayTracer2_0__Point2D__
#define __RayTracer2_0__Point2D__

#include <iostream>
#include "Vector2D.h"

class Point2D : public Vector2D
{
public:
    
    Point2D();
    
    Point2D(double r, double s);
    
    Point2D& operator =(const Vector2D& v);
    
    Point2D& operator *=(double t);
    
    Point2D& operator /=(double t);
    
    Point2D operator -(void) const;
    
    Point2D operator +(const Vector2D& v) const;
    
    Point2D operator -(const Vector2D& v) const;
    
    Vector2D operator -(const Point2D& p) const;
    
    Point2D operator *(double t) const;
    
    Point2D operator /(double t) const;
};

struct Origin2D_
{
    const Point2D& operator +(const Vector2D& v)
    {
        return (static_cast<const Point2D&>(v));
    }
    
    Point2D operator -(const Vector2D& v)
    {
        return (Point2D(-v.x, -v.y));
    }
};

inline Point2D operator *(double t, const Point2D& p)
{
    return (Point2D(t * p.x, t * p.y));
}

extern Origin2D_ Origin2D;

#endif /* defined(__RayTracer2_0__Point2D__) */
