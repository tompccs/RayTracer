#ifndef __RayTracer2_0__Process__
#define __RayTracer2_0__Process__

#include <iostream>
#include <vector>
#include "List.h"
#include "Functions.h"

using namespace std;

class Process{
    List data;
    Functions calc;
    vector<double> Wavelengths, ExtinctionRateValues;
    vector<double> Wavelengthvalues, QuantumYieldValues, ProbabilityValues;
public:
    
    void ReadData(bool evenspaced);
    
    double GetExtinctionRate(double& wavelength);
    
    double GetPathLength(double& wavelength, double& concentration);
    
    double GetEmissionWavelength();
    
    bool QuantumYieldCheck(double& Wavelength);
};

#endif