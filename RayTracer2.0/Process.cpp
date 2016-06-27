#include "Process.h"

void //Reads data from files
Process::ReadData(bool evenspaced, bool hybrid, double radius, bool hot){
    if(!hybrid){
        if(!evenspaced){
            Wavelengthvalues = data.read("data/emission_lambda.txt");
            QuantumYieldValues = data.read("data/emission_quantumyield.txt");
            vector<double> yvalues = data.read("data/emission_AU.txt");
            ProbabilityValues = data.auconvert(yvalues);
            
            vector<double> xvalues = data.read("data/absoption_lambda.txt");
            vector<double> newyvalues = data.read("data/absorption_extinctionrate.txt");
            vector<double>Wavelengths = data.newxdata(xvalues);
            
            ExtinctionRateValues = data.interp1(xvalues, newyvalues, Wavelengths);
            
            vector<double> Sxvalues = data.read("data/hybrid/z-HOT-Wavelengths(300,10,2500).txt");
            vector<double> Snewyvalues = data.read("data/absorption_extinctionrate.txt");
            vector<double> SWavelengths = data.newxdata(xvalues);
            ScatterRateValues = data.interp1(xvalues, newyvalues, Wavelengths);
            
        }
        else{
            Wavelengthvalues = data.read("data/emission_lambda.txt");
            QuantumYieldValues = data.read("data/emission_quantumyield.txt");
            vector<double> yvalues = data.read("data/emission_AU.txt");
            ProbabilityValues = data.auconvert(yvalues);
            
            Wavelengths = data.read("data/absoption_lambda.txt");
            ExtinctionRateValues = data.read("data/absorption_extinctionrate.txt");
            
            lambda_Scatter = data.read("data/absoption_lambda.txt");
            ScatterRateValues = data.read("data/absorption_extinctionrate.txt");
            
        }
    }else{
        if(!evenspaced){
            ostringstream linkabs;
            ostringstream linkscat;
            if(hot){
                linkabs <<"data/hybrid/hot/r"<<radius<<"abshot.txt";
                linkscat <<"data/hybrid/hot/r"<<radius<<"scathot.txt";
            }else{
                linkabs<<"data/hybrid/cold/r"<<radius<<"abscold.txt";
                linkscat<<"data/hybrid/cold/r"<<radius<<"scatcold.txt";

            }
            
            string scat = linkscat.str();
            string abs = linkabs.str();
            
            
            Wavelengthvalues = data.read("data/hybrid/z-HOT-Wavelengths(300,10,2500).txt");
            QuantumYieldValues = data.read("data/emission_quantumyield.txt");
            vector<double> yvalues = data.read("data/emission_AU.txt");
            ProbabilityValues = data.auconvert(yvalues);
            
            vector<double> xvalues = data.read("data/hybrid/z-HOT-Wavelengths(300,10,2500).txt");
            vector<double> newyvalues = data.read(abs);
            vector<double>Wavelengths = data.newxdata(xvalues);
            
            ExtinctionRateValues = data.interp1(xvalues, newyvalues, Wavelengths);
            
            vector<double> Sxvalues = data.read("data/hybrid/z-HOT-Wavelengths(300,10,2500).txt");
            vector<double> Snewyvalues = data.read(scat);
            vector<double> SWavelengths = data.newxdata(xvalues);
            ScatterRateValues = data.interp1(xvalues, newyvalues, Wavelengths);
            
        }
        else{
            
            ostringstream linkabs;
            ostringstream linkscat;
            if(hot){
                linkabs <<"data/hybrid/hotr"<<radius<<"/r"<<radius<<"abshot.txt";
                linkscat <<"data/hybrid/hotr"<<radius<<"/r"<<radius<<"scathot.txt";
            }else{
                linkabs<<"data/hybrid/coldr"<<radius<<"/r"<<radius<<"abscold.txt";
                linkscat<<"data/hybrid/coldr"<<radius<<"/r"<<radius<<"scatcold.txt";
                
            }
            
            string scat = linkscat.str();
            string abs = linkabs.str();

            Wavelengthvalues = data.read("data/hybrid/z-HOT-Wavelengths(300,10,2500).txt");
            QuantumYieldValues = data.read("data/emission_quantumyield.txt");
            vector<double> yvalues = data.read("data/emission_AU.txt");
            ProbabilityValues = data.auconvert(yvalues);
            
            Wavelengths = data.read("data/hybrid/z-HOT-Wavelengths(300,10,2500).txt");
            ExtinctionRateValues = data.read(abs);
            
            lambda_Scatter = data.read("data/hybrid/z-HOT-Wavelengths(300,10,2500).txt");
            ScatterRateValues = data.read(scat);
            
        }
    }
}

double //Gets extinction rates
Process::GetExtinctionRate(double &wavelength){
    double i = data.GetAfromB(wavelength, ExtinctionRateValues, Wavelengths);
    //cout<<i<<endl;
    return i;
}

double //Gets extinction rates
Process::GetScatterRate(double &wavelength){
    double i = data.GetAfromB(wavelength, ScatterRateValues, lambda_Scatter);
    //cout<<i<<endl;
    return i;
}

double //gets path lengths
//ExtinctionRate is actually 'Molar attenuation coefficient' in units of M^-1 * cm^-1
Process::GetPathLength(double &wavelength, double &concentration){
    double beta = concentration*GetExtinctionRate(wavelength);
    //cout<<beta<<endl;
    if(beta!=0){
        double pathlength = -(log(calc.Random(1))/beta);
        if(pathlength>1e-9) return pathlength;
    }
    return INFINITY;
}

double //gets path lengths
//ExtinctionRate is actually 'Molar attenuation coefficient' in units of M^-1 * cm^-1
Process::GetScatterPathLength(double &wavelength, double &concentration){
    double beta = concentration*GetScatterRate(wavelength);
    //cout<<beta<<endl;
    if(beta!=0){
        double pathlength = -(log(calc.Random(1))/beta);
        if(pathlength>1e-9) return pathlength;
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