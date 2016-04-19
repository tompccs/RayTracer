#ifndef __RayTracer__FresnelJackson__
#define __RayTracer__FresnelJackson__

#include <iostream>
#include "Functions.h"
#include <vector>
#include "Test.h"
#include <cmath>
#include <cfloat>
#include "MultipleObjects.h"
#include "curvedbox.hpp"
#include "flexi.hpp"
#include "curvedlsc.hpp"

class FresnelJackson{
    Vector3D NewMomentum, NewPolarisation;
    bool Transmitted;
    Functions calc;
    Test print;
    bool err;
public:
    void Calculate(Vector3D& OldMomentum, Vector3D& OldPolarisation, Vector3D& theFacetNormal, double& Rindex1, double& Rindex2, bool debug);
    
    void In(Photon* photon, Material* world, Material* lsc, bool& debug, bool scatter);
    
    bool Out(Photon* photon, Material* material2, Material* material1, bool& debug, MultipleObjects* objects, bool scatter);
    
    Vector3D ProjectionOnPlane(Vector3D plane_normal, Vector3D vector);
    
    Vector3D GetNewMomentum();
    Vector3D GetNewPolarisation();
    
    void CurvedIn(Photon* photon, Material* world, curvedbox* FLSC, bool& debug);
    
    void CurvedOut(Photon* photon, curvedbox* FLSC, Material* world, bool& debug);
    
    void FlexiIn(Photon* photon, Material* world, flexi* FLSC, bool& debug);
    
    void FlexiOut(Photon* photon, flexi* FLSC, Material* world, bool& debug);
    
    void NewCurvedIn(Photon* photon, Material* world, curvedlsc& FLSC, bool& debug);
    
    void NewCurvedOut(Photon* photon, curvedlsc& FLSC, Material* world, bool& debug, double& reflections);


};
#endif