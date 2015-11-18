#ifndef __RayTracer2_0__Material__
#define __RayTracer2_0__Material__

#include <iostream>
#include "Box.h"
#include "Process.h"
#include "Test.h"

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
    
    void ReadData(bool evenspaced);
    
    void SetInitialAbsorbLength(Photon* P);
    
    void AbsorptionEvent(Photon* P, bool& debug, bool& matlabprint, vector<Point3D>& dyeabs, vector<Point3D>& photonpath);
    
    void SetPhotonInside(bool inside);
    
    bool ReturnPhotonInside();
    
    void CorrectPhotonInside(Photon* photon);
    
    void SetMaterialNumber(long n);
    
    long& GetMaterialNumber();
    
    void SetCurved(bool yesno);
    
    bool& GetCurved();
    
    
};

#endif