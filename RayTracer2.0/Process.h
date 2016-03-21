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
    vector<double> lambda_Scatter, ScatterRateValues;
public:
    
    void ReadData(bool evenspaced, bool hybrid);
    
    double GetExtinctionRate(double& wavelength);
    
    double GetPathLength(double& wavelength, double& concentration);
    
    double GetEmissionWavelength();
    
    double GetScatterPathLength(double&wavelength, double& concentration);
    
    double GetScatterRate(double& wavelength);
    
    bool QuantumYieldCheck(double& Wavelength);
};

#endif