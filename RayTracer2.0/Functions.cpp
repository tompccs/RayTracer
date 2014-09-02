#include "Functions.h"

double
Functions::GetVectorMin(vector<double> a){
    double min = DBL_MAX;
    for(int i=0; i<a.size(); i++){
        if (a[i]<min)
            min = a[i];
    }
    return min;
}

double
Functions::Random(double a){
    return (a * ((double) rand() / (RAND_MAX)));
}