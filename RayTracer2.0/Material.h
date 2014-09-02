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
    Test print;
public:
    
    Process& GetProcess();
    
    void SetProcess(const Process& n);
    
    double& GetRefractiveIndex();
    
    void SetRefractiveIndex(double n);
    
    double& GetConcentration();
    
    void SetConcentration(double c);
    
    void ReadData(bool evenspaced); //Reads Absorption/Emission Data for material from files.
    
    void SetInitialAbsorbLength(Photon* P); //Gets absorption length from photon wavelength and material concentration.
    
    void AbsorptionEvent(Photon* P, bool& debug); //Absorption event.
    
    void SetPhotonInside(bool inside);
    
    bool ReturnPhotonInside();
    
    void CorrectPhotonInside(Photon* photon);
    
    };

#endif