#include "Vector3D.h"

//Definitions of a vector

Vector3D::Vector3D(){}

Vector3D::Vector3D(double r, double s, double t)
{
    x = r;
    y = s;
    z = t;
}

Vector3D::Vector3D(const Vector2D& v, double u)
{
    x = v.x;
    y = v.y;
    z = u;
}

Vector3D&
Vector3D::Set(double r, double s, double t)
{
    x = r;
    y = s;
    z = t;
    return (*this);
}

Vector3D&
Vector3D::Set(const Vector2D& v, double u)
{
    x = v.x;
    y = v.y;
    z = u;
    return (*this);
}

double&
Vector3D::operator [](long k)
{
    return ((&x)[k]);
}

const double&
Vector3D::operator [](long k) const
{
    return ((&x)[k]);
}

Vector3D&
Vector3D::operator =(const Vector2D& v)
{
    x = v.x;
    y = v.y;
    z = 0.0F;
    return (*this);
}

Vector3D&
Vector3D::operator +=(const Vector3D& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return (*this);
}

Vector3D&
Vector3D::operator +=(const Vector2D& v)
{
    x += v.x;
    y += v.y;
    return (*this);
}

Vector3D&
Vector3D::operator -=(const Vector3D& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return (*this);
}

Vector3D&
Vector3D::operator -=(const Vector2D& v)
{
    x -= v.x;
    y -= v.y;
    return (*this);
}

Vector3D&
Vector3D::operator *=(double t)
{
    x *= t;
    y *= t;
    z *= t;
    return (*this);
}

Vector3D&
Vector3D::operator /=(double t)
{
    double f = 1.0F / t;
    x *= f;
    y *= f;
    z *= f;
    return (*this);
}


Vector3D&
Vector3D::operator %=(const Vector3D& v)
{
    double       r, s;
    
    r = y * v.z - z * v.y;
    s = z * v.x - x * v.z;
    z = x * v.y - y * v.x;
    x = r;
    y = s;
    
    return (*this);
}

Vector3D&
Vector3D::operator &=(const Vector3D& v)
{
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return (*this);
}

Vector3D
Vector3D::operator -(void) const
{
    return (Vector3D(-x, -y, -z));
}

Vector3D
Vector3D::operator +(const Vector3D& v) const
{
    return (Vector3D(x + v.x, y + v.y, z + v.z));
}

Vector3D
Vector3D::operator +(const Vector2D& v) const
{
    return (Vector3D(x + v.x, y + v.y, z));
}


Vector3D
Vector3D::operator -(const Vector3D& v) const
{
    return (Vector3D(x - v.x, y - v.y, z - v.z));
}

Vector3D
Vector3D::operator -(const Vector2D& v) const
{
    return (Vector3D(x - v.x, y - v.y, z));
}

Vector3D
Vector3D::operator *(double t) const
{
    return (Vector3D(x * t, y * t, z * t));
}

Vector3D
Vector3D::operator /(double t) const
{
    double f = 1.0F / t;
    return (Vector3D(x * f, y * f, z * f));
}

double
Vector3D::operator *(const Vector3D& v) const
{
    return (x * v.x + y * v.y + z * v.z);
}

double
Vector3D::operator *(const Vector2D& v) const
{
    return (x * v.x + y * v.y);
}

Vector3D
Vector3D::operator %(const Vector3D& v) const
{
    return (Vector3D(y * v.z - z * v.y, z * v.x - x * v.z,
                     x * v.y - y * v.x));
}

Vector3D
Vector3D::operator &(const Vector3D& v) const
{
    return (Vector3D(x * v.x, y * v.y, z * v.z));
}

bool
Vector3D::operator ==(const Vector3D& v) const
{
    return ((x == v.x) && (y == v.y) && (z == v.z));
}

bool
Vector3D::operator !=(const Vector3D& v) const
{
    return ((x != v.x) || (y != v.y) || (z != v.z));
}

Vector3D&
Vector3D::Normalise(void)
{
    return (*this /= sqrtf(x * x + y * y + z * z));
}

Vector3D&
Vector3D::RotateAboutX(double angle)
{
    double s = sinf(angle);
    double c = cosf(angle);
    
    double ny = c * y - s * z;
    double nz = c * z + s * y;
    
    y = ny;
    z = nz;
    
    return (*this);
}

Vector3D&
Vector3D::RotateAboutY(double angle)
{
    double s = sinf(angle);
    double c = cosf(angle);
    
    double nx = c * x + s * z;
    double nz = c * z - s * x;
    
    x = nx;
    z = nz;
    
    return (*this);
}

Vector3D&
Vector3D::RotateAboutZ(double angle)
{
    double s = sinf(angle);
    double c = cosf(angle);
    
    double nx = c * x - s * y;
    double ny = c * y + s * x;
    
    x = nx;
    y = ny;
    
    return (*this);
}

Vector3D&
Vector3D::RotateAboutAxis(double angle, const Vector3D& axis)
{
    double s = sinf(angle);
    double c = cosf(angle);
    double k = 1.0F - c;
    
    double nx = x * (c + k * axis.x * axis.x) + y * (k * axis.x * axis.y - s * axis.z)
    + z * (k * axis.x * axis.z + s * axis.y);
    double ny = x * (k * axis.x * axis.y + s * axis.z) + y * (c + k * axis.y * axis.y)
    + z * (k * axis.y * axis.z - s * axis.x);
    double nz = x * (k * axis.x * axis.z - s * axis.y) + y * (k * axis.y * axis.z + s * axis.x)
    + z * (c + k * axis.z * axis.z);
    
    x = nx;
    y = ny;
    z = nz;
    
    return (*this);
}

Vector3D
Vector3D::GetRandomUnitVector(){
    double cost = 1 - 2* calc.Random(1);
    double sint = sqrt(1 - cost * cost);
    double phi = 2 * M_PI * calc.Random(1);
    double sinp = sin(phi);
    double cosp = cos(phi);
    
    double px = sint*cosp;
    double py = sint*sinp;
    double pz = cost;
    
    return Vector3D(px,py,pz);
}
