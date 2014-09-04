#ifndef __RayTracer__FresnelJackson__
#define __RayTracer__FresnelJackson__

#include <iostream>
#include "Functions.h"
#include <vector>
#include "Test.h"
#include <cmath>
#include <cfloat>
#include "MultipleObjects.h"

class FresnelJackson{
    Vector3D NewMomentum, NewPolarisation;
    bool Transmitted;
    Functions calc;
    Test print;
public:
    void Calculate(Vector3D& OldMomentum, Vector3D& OldPolarisation, Vector3D& theFacetNormal, double& Rindex1, double& Rindex2);
    
    void In(Photon* photon, Material* world, Material* lsc, bool& debug);
    //Refract/Reflect event from Outside Material to Inside.
    
    void Out(Photon* photon, Material* material2, Material* material1, bool& debug, MultipleObjects* objects);
    //Refract/Reflect event from Inside Material to Outside.

    Vector3D ProjectionOnPlane(Vector3D plane_normal, Vector3D vector);
};
#endif