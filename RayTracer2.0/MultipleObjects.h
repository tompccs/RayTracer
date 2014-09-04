#ifndef __RayTracer__MultipleObjects__
#define __RayTracer__MultipleObjects__

#include <iostream>
#include <vector>
#include "Material.h"
#include "Functions.h"

class MultipleObjects{
    
    vector<Material*>  ObjectList;
    Material* world;
    Functions calc;
    
public:
    
    void StoreMaterial(Material* object);
    
    void StoreWorld(Material* WORLD);
    
    void PrintSize();
    
    Material*& CurrentMaterial();
    
    bool PhotonInMaterial();
    
    double NextInterfaceDistance(Photon* photon);
    
    Material*& NextInterfaceMaterial(Photon* photon);
};

#endif