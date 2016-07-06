#ifndef spectra_hpp
#define spectra_hpp

#include <stdio.h>
#include "matrixread.hpp"
#include "List.h"
#include "Functions.h"

class spectra{
    matrixread mat;
    List l;
    vector<double> Wavelengths;
    vector<double> Intensity;
    Functions generator;
public:
    void Import(const char  *filename);
    matrixread& GetMatrix();
    vector<double> RedistributeXVals();
    vector<double> RedistributeYVals();
    vector<vector<double>> TrimWavelengths(double start, double end);
    double GetValuefromWavelength(double wavelength, vector<double>& lambdas, vector<double>& intensities);
    double GetWavelengthfromValue(double probability, vector<double>& lambdas, vector<double>& probabilities);
    vector<double>& NormaliseProbabilities(vector<double>& vect);
    
    void Setup(double startwavelength, double endwavelength, const char *filename);
    double GenerateWavelength();
};

#endif