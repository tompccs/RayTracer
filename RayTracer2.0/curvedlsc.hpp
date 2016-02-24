#ifndef curved_lsc_hpp
#define curved_lsc_hpp

#include <stdio.h>
#include "arch.hpp"
#include "Sheet.h"
#include "Intersection.hpp"
#include "Material.h"

class curvedlsc: public Material{
    
    arch inside;
    arch outside;
    Sheet top;
    Sheet bottom;
    Sheet start;
    Sheet end;
    
    double refractiveindex;
    //bool Photoninside;
    
    vector<Intersection> intersections;
    
public:
    curvedlsc();
    void Set(Point3D centre, double radius, double length, double thickness, double height, double refractiveindex);
    void FindIntersections(Photon& p, bool debug);
    int NextIntersection(Photon& p, bool debug);
    double NextDistance(Photon& p, bool debug);
    Vector3D NextNormal(Photon& p, bool debug);
    
    double& GetRefractiveIndex();
    void SetRefractiveIndex(double n);
    
    //void SetPhotonInside(bool inside);
    //bool& GetPhotonInside();
    
    void PrintNextInterface(Photon& p, bool debug);
    
    Sheet& GetSheet(int sheetnumber, bool debug);

    
    
};

#endif