#ifndef flexi_hpp
#define flexi_hpp

#include <stdio.h>
#include "Point3D.h"
#include "ellipse.hpp"
#include "Sheet.h"
#include "Material.h"

class flexi: public Material{
    
    Point3D centre; //Central point of Ellipse
    double t; // Thickness of curved LSC
    double l; // Length of LSC.
    double h; // Height of LSC.
    double r; // radius of curvature
    double n; // Refractive index.
    
    bool PhotonInside;
    
    ellipse inside; // Ellipse describing line on which inside sheet lies
    ellipse outside; // Ellipse describing line on which outside sheet lies
    Sheet start; // Start sheet
    Sheet end; // End sheet
    Sheet top; // Top sheet
    Sheet bottom; //Bottom Sheet
    
    double startangle;
    double endangle;
    
    double insidedistance;
    double outsidedistance;
    double startdistance;
    double enddistance;
    double topdistance;
    double bottomdistance;
    
    int NextInterface;
    Vector3D NextNormal;
    
public:
    
    flexi();
    
    void Set(Point3D& C, double& T, double& L, double& H, double& R, double &N);
    
    void SetCentre(Point3D& point);
    Point3D& GetCentre();
    
    void SetThickness(double& number);
    double& GetThickness();
    
    void SetLength(double& number);
    double& GetLength();
    
    void SetHeight(double& number);
    double& GetHeight();
    
    void SetRadius(double& number);
    double& GetRadius();
    
    void SetRefractiveIndex(double& number);
    double& GetRefractiveIndex();
    
    void SetPhotonInside(bool& b);
    bool& GetPhotonInside();
    
    int& GetNextInterface();
    
    void Build(); //Builds walls of LSC based on parameters given using 'Set' methods
    
    double DistancetoCurvedIntersect(Photon& photon, ellipse& ell);
    
    double CurvedDet(Photon& photon, ellipse& ell);
    Point3D FlatPointOne(Photon& photon, ellipse& ell, double& det);
    Point3D FlatPointTwo(Photon& photon, ellipse& ell, double& det);
    
    Point3D Project3D(Photon& photon, Point3D& point);
    
    double FindAngle(Point3D& point, ellipse& ell);
    
    bool pointonpath(Point3D& point, Photon& photon);
    
    bool directioncheck(Vector3D& v, Vector3D& m);
    
    void CalculateDistances(Photon& photon);
    
    double IntersectionDistance();
    
    Vector3D CurvedNormal(Photon& photon, ellipse& ell);
    
    Vector3D GetNextNormal(Photon& photon);
};

#endif