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
    
public:
    
    curvedbox();
    curvedbox(arc& central, double height, double width, double refractiveindex);
    
    double DToStartSheet(Photon& photon);
    double DToEndSheet(Photon& photon);
    double DToInsideArc(Photon& photon);
    double DToOutsideArc(Photon& photon);
    double DToTopSheet(Photon& photon);
    double DToBottomSheet(Photon& photon);
    
    bool TopBottomSheetPointCheck(Point3D& point);
    
    void Set(arc central, double height, double width, double refractiveindex);
    
    int NextInterface(Photon& photon);
    
    double NextInterfaceDistance(Photon& photon);
    
    Vector3D GetNextNormal(Photon& photon);
    
    double& GetRefractiveIndex();
    
    void SetPhotonInside(bool inside);
    
    bool GetPhotonInside();
    
    
};

#endif