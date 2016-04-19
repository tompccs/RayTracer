#ifndef __RayTracer2_0__Material__
#define __RayTracer2_0__Material__

#include <iostream>
#include "Box.h"
#include "Process.h"
#include "Test.h"
#include "scattering.hpp"

class Material: public Box{
    Process Events;
    double RefractiveIndex, Concentration;
    bool PhotonInside;
    long Materialnumber;
    Test print;
    bool Curved;
public:
    
    Process& GetProcess();
    
    void SetProcess(const Process& n);
    
    double& GetRefractiveIndex();
    
    void SetRefractiveIndex(double n);
    
    double& GetConcentration();
    
    void SetConcentration(double c);
    
    void ReadData(bool evenspaced, bool hybrid, double radius, bool hot);
    
    void SetInitialScatterLength(Photon* P);
    
    void ScatterEvent(Photon* P, bool& debug, bool& matlabprint, vector<Point3D>& dyeabs, vector<Point3D>& photonpath, scattering& sc, bool& hot, double& radius);

    
    void SetInitialAbsorbLength(Photon* P);
    
    void AbsorptionEvent(Photon* P, bool& debug, bool& matlabprint, vector<Point3D>& dyeabs, vector<Point3D>& photonpath, bool& hot, double& radius);
    
    void SetPhotonInside(bool inside);
    
    bool ReturnPhotonInside();
    
    void CorrectPhotonInside(Photon* photon);
    
    void SetMaterialNumber(long n);
    
    long& GetMaterialNumber();
    
    void SetCurved(bool yesno);
    
    bool& GetCurved();
    
    
};

#endif