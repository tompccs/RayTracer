#ifndef curvedbox_hpp
#define curvedbox_hpp

#include <stdio.h>
#include "Sheet.h"
#include "Photon.h"
#include "Vector3D.h"
#include "arc.hpp"
#include "Material.h"
#include "Test.h"

class curvedbox: public Material{
    arc insidearc;
    arc outsidearc;
    
    Sheet startsheet;
    Sheet endsheet;
    Sheet topplane;
    Sheet bottomplane;
    
    double refractiveindex;
    bool PhotonInside;
    int NextInterfaceValue;
    
public:
    
    curvedbox();
    curvedbox(arc& central, double height, double width, double refractiveindex);
    
    double DToStartSheet(Photon& photon);
    double DToEndSheet(Photon& photon);
    double DToInsideArc(Photon& photon, bool debug);
    double DToOutsideArc(Photon& photon, bool debug);
    double DToTopSheet(Photon& photon);
    double DToBottomSheet(Photon& photon);
    
    bool TopBottomSheetPointCheck(Point3D& point);
    
    void Set(arc central, double height, double width, double refractiveindex);
    
    int NextInterface(Photon& photon, bool debug);
    
    double NextInterfaceDistance(Photon& photon, bool debug);
    
    Vector3D GetNextNormal(Photon& photon, bool debug);
    
    double& GetRefractiveIndex();
    
    void SetPhotonInside(bool inside);
    
    bool GetPhotonInside();
    
    void Intersection(Photon& photon);
    
    Sheet& GetStartSheet();
    Sheet& GetEndSheet();
    
    
};

#endif