#include "Point2D.h"

//Definitions of a Point.

Origin2D_ Origin2D;

Point2D::Point2D(){}

Point2D::Point2D(double r, double s):
Vector2D(r,s)
{}

Point2D&
Point2D::operator =(const Vector2D& v)
{
    x = v.x;
    y = v.y;
    return (*this);
}

Point2D&
Point2D::operator *=(double t)
{
    x *= t;
    y *= t;
    return (*this);
}

Point2D&
Point2D::operator /=(double t)
{
    double f = 1.0F / t;
    x *= f;
    y *= f;
    return (*this);
}

Point2D
Point2D::operator -(void) const
{
    return (Point2D(-x, -y));
}

Point2D
Point2D::operator +(const Vector2D& v) const
{
    return (Point2D(x + v.x, y + v.y));
}

Point2D
Point2D::operator -(const Vector2D& v) const
{
    return (Point2D(x - v.x, y - v.y));
}

Vector2D
Point2D::operator -(const Point2D& p) const
{
    return (Vector2D(x - p.x, y - p.y));
}

Point2D
Point2D::operator *(double t) const
{
    return (Point2D(x * t, y * t));
}

Point2D
Point2D::operator /(double t) const
{
    double f = 1.0F / t;
    return (Point2D(x * f, y * f));
}