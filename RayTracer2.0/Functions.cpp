#include "Functions.h"



double //Returns minimum value from a vector of numbers
Functions::GetVectorMin(vector<double> a){
    double min = DBL_MAX;
    for(int i=0; i<a.size(); i++){
        if (a[i]<min)
            min = a[i];
    }
    return min;
}

double //Generates random number between 0 and a
Functions::Random(double a){
    double number = (a * ((double) rand() / (RAND_MAX)));
    return number ;
}

double
Functions::GetAverage(vector<double>& a){
    double average = 0;
    
    
    return average;
}