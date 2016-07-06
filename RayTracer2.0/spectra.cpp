#include "spectra.hpp"

void
spectra::Import(const char *filename){
    mat.Data(filename);
}

matrixread&
spectra::GetMatrix(){
    return mat;
}

vector<double>
spectra::RedistributeXVals(){
    vector<double> xvals;
    for(int i=0; i<mat.GetMatrix().size(); i++){
        xvals.push_back(mat.GetMatrix()[i][0]);
    }
    
    vector<double> newvals = l.newxdata(xvals);
    
    return newvals;
}

vector<double>
spectra::RedistributeYVals(){
    vector<double> yvals;
    for(int i=0; i<mat.GetMatrix().size(); i++){
        yvals.push_back(mat.GetMatrix()[i][1]);
    }
    
    vector<double> xvals = RedistributeXVals();
    vector<double> newyvals;
    l.interp1(xvals, yvals, newyvals);
    
    return newyvals;
}

vector<vector<double>>
spectra::TrimWavelengths(double start, double end){
    vector<double> lambda;
    vector<double> intensity;
    for(int i=0; i<mat.GetMatrix().size(); i++){
        if(mat.GetMatrix()[i][0]>start && mat.GetMatrix()[i][0]<end){
            lambda.push_back(mat.GetMatrix()[i][0]);
            intensity.push_back(mat.GetMatrix()[i][1]);
        
        }
        
    }
    vector<vector<double>> m;
    m.push_back(lambda);
    
    Wavelengths = lambda;
    
    m.push_back(intensity);

    Intensity = intensity;
    
    return m;
}

double
spectra::GetValuefromWavelength(double wavelength, vector<double> &lambdas, vector<double> &intensities){
    double v = l.GetAfromB(wavelength, intensities, lambdas);
    return v;
}

double
spectra::GetWavelengthfromValue(double probability, vector<double> &lambdas, vector<double> &probabilities){    double v = l.GetAfromB(probability, lambdas, probabilities);
    return v;
}

vector<double>&
spectra::NormaliseProbabilities(vector<double> &vect){
    for(int i=1; i<vect.size(); i++){
        vect[i] = vect[i]+vect[i-1];
    }
    
    double total = vect[vect.size()-1];
    
    for(int i=0; i<vect.size(); i++){
        vect[i] = vect[i]/total;
    }
    
    return vect;
}

void
spectra::Setup(double startwavelength, double endwavelength, const char *filename){
    Import(filename);
    TrimWavelengths(startwavelength, endwavelength);
    NormaliseProbabilities(Intensity);
}

double
spectra::GenerateWavelength(){
    double wavelength = 0;
    double NumGen = generator.Random(1);
    wavelength = GetWavelengthfromValue(NumGen, Wavelengths, Intensity);
    return wavelength;
}
