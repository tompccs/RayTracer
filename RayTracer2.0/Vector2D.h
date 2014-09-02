#ifndef __RayTracer2_0__Vector2D__
#define __RayTracer2_0__Vector2D__

#include <iostream>
#include <math.h>


class Vector2D
{
public:
    
    double   x;
    double   y;
    
    Vector2D();
    
    Vector2D(double r, double s);
    
    Vector2D& Set(double r, double s);
    
    double& operator [](long k);
    
    const double& operator [](long k) const;
    
    Vector2D& operator +=(const Vector2D& v);
    
    Vector2D& operator -=(const Vector2D& v);
    
    Vector2D& operator *=(double t);
    
    Vector2D& operator /=(double t);
    
    Vector2D& operator &=(const Vector2D& v);
    
    Vector2D operator -(void) const;
    
    Vector2D operator +(const Vector2D& v) const;
    
    Vector2D operator -(const Vector2D& v) const;
    
    Vector2D operator *(double t) const;
    
    Vector2D operator /(double t) const;
    
    double operator *(const Vector2D& v) const;
    
    Vector2D operator &(const Vector2D& v) const;
    
    bool operator ==(const Vector2D& v) const
    {
        return ((x == v.x) && (y == v.y));
    }
    
    bool operator !=(const Vector2D& v) const;
    
    Vector2D& Normalize(void);
    
    Vector2D& Rotate(double angle);
};

inline Vector2D operator *(double t, const Vector2D& v)
{
    return (Vector2D(t * v.x, t * v.y));
}


inline double Dot(const Vector2D& v1, const Vector2D& v2)
{
    return (v1 * v2);
}

inline double Magnitude(const Vector2D& v)
{
    return (sqrtf(v.x * v.x + v.y * v.y));
}

inline double InverseMag(const Vector2D& v)
{
    return (1.0F / sqrtf(v.x * v.x + v.y * v.y));
}

inline double SquaredMag(const Vector2D& v)
{
    return (v.x * v.x + v.y * v.y);
}

#endif /* defined(__RayTracer2_0__Vector2D__) */
