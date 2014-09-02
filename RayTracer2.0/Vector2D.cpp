#include "Vector2D.h"

Vector2D::Vector2D(){}

Vector2D::Vector2D(double r, double s){
    x = r;
    y = s;
}

Vector2D&
Vector2D::Set(double r, double s)
{
    x = r;
    y = s;
    return (*this);
}

double&
Vector2D::operator [](long k)
{
    return ((&x)[k]);
}

const double&
Vector2D::operator [](long k) const
{
    return ((&x)[k]);
}

Vector2D&
Vector2D::operator +=(const Vector2D& v)
{
    x += v.x;
    y += v.y;
    return (*this);
}

Vector2D&
Vector2D::operator -=(const Vector2D& v)
{
    x -= v.x;
    y -= v.y;
    return (*this);
}

Vector2D&
Vector2D::operator *=(double t)
{
    x *= t;
    y *= t;
    return (*this);
}

Vector2D&
Vector2D::operator /=(double t)
{
    double f = 1.0F / t;
    x *= f;
    y *= f;
    return (*this);
}

Vector2D&
Vector2D::operator &=(const Vector2D& v)
{
    x *= v.x;
    y *= v.y;
    return (*this);
}

Vector2D
Vector2D::operator -(void) const
{
    return (Vector2D(-x, -y));
}

Vector2D
Vector2D::operator +(const Vector2D& v) const
{
    return (Vector2D(x + v.x, y + v.y));
}

Vector2D
Vector2D::operator -(const Vector2D& v) const
{
    return (Vector2D(x - v.x, y - v.y));
}

Vector2D
Vector2D::operator *(double t) const
{
    return (Vector2D(x * t, y * t));
}

Vector2D
Vector2D::operator /(double t) const
{
    double f = 1.0F / t;
    return (Vector2D(x * f, y * f));
}

double
Vector2D::operator *(const Vector2D& v) const
{
    return (x * v.x + y * v.y);
}

Vector2D
Vector2D::operator &(const Vector2D& v) const
{
    return (Vector2D(x * v.x, y * v.y));
}

bool
Vector2D::operator !=(const Vector2D& v) const
{
    return ((x != v.x) || (y != v.y));
}

Vector2D&
Vector2D::Normalize(void)
{
    return (*this /= sqrtf(x * x + y * y));
}

Vector2D&
Vector2D::Rotate(double angle)
{
    double s = sinf(angle);
    double c = cosf(angle);
    
    double nx = c * x - s * y;
    double ny = s * x + c * y;
    
    x = nx;
    y = ny;
    
    return (*this);
}