#include "Process.h"

void //Reads data from files
Process::ReadData(bool evenspaced){
    if(!evenspaced){
        Wavelengthvalues = data.read("/Users/misha/Documents/IoannisGroup/RayTracer2.0/RayTracer2.0/data/emission_lambda.txt");
        QuantumYieldValues = data.read("/Users/misha/Documents/IoannisGroup/RayTracer2.0/RayTracer2.0/data/emission_quantumyield.txt");
        vector<double> yvalues = data.read("/Users/misha/Documents/IoannisGroup/RayTracer2.0/RayTracer2.0/data/emission_AU.txt");
        ProbabilityValues = data.auconvert(yvalues);
        
        vector<double> xvalues = data.read("/Users/misha/Documents/IoannisGroup/RayTracer2.0/RayTracer2.0/data/absoption_lambda.txt");
        vector<double> newyvalues = data.read("/Users/misha/Documents/IoannisGroup/RayTracer2.0/RayTracer2.0/data/absorption_extinctionrate.txt");
        Wavelengths = data.newxdata(xvalues);
        ExtinctionRateValues = data.interp1(xvalues, newyvalues, Wavelengths);
    }
    else{
        Wavelengthvalues = data.read("/Users/misha/Documents/IoannisGroup/RayTracer2.0/RayTracer2.0/data/emission_lambda.txt");
        QuantumYieldValues = data.read("/Users/misha/Documents/IoannisGroup/RayTracer2.0/RayTracer2.0/data/emission_quantumyield.txt");
        vector<double> yvalues = data.read("/Users/misha/Documents/IoannisGroup/RayTracer2.0/RayTracer2.0/data/emission_AU.txt");
        ProbabilityValues = data.auconvert(yvalues);
        
        Wavelengths = data.read("/Users/misha/Documents/IoannisGroup/RayTracer2.0/RayTracer2.0/data/absoption_lambda.txt");
        ExtinctionRateValues = data.read("/Users/misha/Documents/IoannisGroup/RayTracer2.0/RayTracer2.0/data/absorption_extinctionrate.txt");
    }
}

double //Gets extinction rates
Process::GetExtinctionRate(double &wavelength){
    double i = data.GetAfromB(wavelength, ExtinctionRateValues, Wavelengths);
    //cout<<i<<endl;
    return i;
}

double //gets path lengths
Process::GetPathLength(double &wavelength, double &concentration){
    double beta = concentration*GetExtinctionRate(wavelength);
    //cout<<beta<<endl;
    if(beta!=0){
        return -(log(calc.Random(1))/beta);
    }
    return INFINITY;
}

double //gets wavelength of emission
Process::GetEmissionWavelength(){
    return data.GetAfromB(calc.Random(1), Wavelengthvalues, ProbabilityValues);
}

bool //checks if reemitted
Process::QuantumYieldCheck(double &Wavelength){
    //   int i = data.findNearestNeighbourIndex(Wavelength, Wavelengthvalues);
    if(calc.Random(1)<=0.8) return 1;
    //if(calc.Random(1)<=QuantumYieldValues[i]) return 1; //ADD FILE WITH ALL QY VALUES
    else return 0;
    
}