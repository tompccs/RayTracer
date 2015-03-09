#ifndef __RayTracer__anisotropic__
#define __RayTracer__anisotropic__

#include <stdio.h>
#include "matrix.h"
#include <iostream>
#include <random>
#include <math.h>
#include <cmath>
#include "Spherical3D.h"
#include "Functions.h"

using namespace std;

class anisotropic{
    
public:
    matrix<double> probs(int thetalength, int philength, int rolls, int deviation);
    
    matrix<double> cumulative(matrix<double>& oldmatrix);
    
    Spherical3D pointlookup(matrix<double>& probabilities);
    
    matrix<double> genvector();
    
    
};


#endif