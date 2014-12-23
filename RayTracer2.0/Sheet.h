#ifndef __RayTracer2_0__Sheet__
#define __RayTracer2_0__Sheet__

#include <iostream>
#include <iomanip>
#include "Point3D.h"
#include "Photon.h"

class Sheet{
    Point3D A,B,C;
    Vector3D normal;
    bool obtainednormal;

public:
    Sheet();
    Sheet(const Point3D& a, const Point3D& b, const Point3D& c);
    Sheet(const Sheet& sheet);
    ~Sheet();
    
    Sheet& Set(const Point3D& a, const Point3D& b, const Point3D& c);
    Sheet& Get();
    void SetA(const Point3D& a);
    void SetB(const Point3D& b);
    void SetC(const Point3D& c);
    Point3D& GetA();
    Point3D& GetB();
    Point3D& GetC();
    Point3D GetD();
    double GetABLength();
    double GetACLength();
    double GetArea();
    Vector3D GetNormal();
    bool PhotonPlaneCheck(Photon* photon);
    Point3D IntersectionPoint(Photon* photon);
    double IntersectionDistance(Photon* photon);
    bool PointonSheetTest(const Point3D& p);
    bool GetIntersectionTest(Photon* photon);
    bool operator == (const Sheet& s);
    Point3D LinePlaneIntersectPoint(Photon *photon);
    bool PointSheetCheck(Point3D point);
    double TriangleArea(Point3D A, Point3D B, Point3D C);
    bool SignCompare(double A, double B, double C);
    Point3D Centre();
};

#endif