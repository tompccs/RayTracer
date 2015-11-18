#include "anisotropic.h"


/*This method generates a matrix full of probabilities. It has xlength 'thetalength
 and ylength 'philength'. The distribution is normal with values centred about theta = 0
 or pi, and is independant of phi. the number of random rolls is 'rolls'.
 */
matrix<double>
anisotropic::probs(int thetalength, int philength, int rolls, int deviation){
    
    matrix<double> values(thetalength,philength,0);
    
    const int nrolls=rolls;  //number of experiments
    
    const double mean = 0;
    const double stddeviation = deviation;
    
    default_random_engine generator;
    normal_distribution<double> distribution(mean,stddeviation);
    
    double* p = new double[thetalength/2];
    
    for (int i=0; i<rolls; ++i) {
        double number = abs(distribution(generator));
        if ((number>=0.0)&&(number<thetalength/2)) ++p[int(number)];
    }
    
    
    for (int i=0; i<thetalength/2; i++) {
        values(i,0)=((p[i]/nrolls)/philength)/2;
    }
    
    for (int i=thetalength/2; i<thetalength; i++){
        values(i,0)=((p[thetalength-i-1]/nrolls)/philength)/2;
    }
    
    for (int i=0; i<thetalength; i++){
        for(int j=0; j<philength; j++){
            values(i,j) = values(i,0);
        }
    }
    
    
    double total=0;
    
    for(int i=0; i<thetalength; i++){
        for(int j=0; j<philength; j++){
            total = total + values(i,j);
        }
    }
    
    cout<<"Total probability is: "<<total<<endl;
    
    
    
    return values;
}

/* This method converts a matrix of probabilities into one of cumulative probabilities.
 It reads the matrix like a book, across the rows and then down the colomns.*/

matrix<double>
anisotropic::cumulative(matrix<double> &oldmatrix){
    matrix<double> newmatrix(oldmatrix);
    
    for(int i=0; i<newmatrix.get_rows();i++){
        if(i>0){
            newmatrix(i,0)= newmatrix(i,0) + newmatrix(i-1,newmatrix.get_cols()-1);
        }
        for(int j=1; j<newmatrix.get_cols();j++){
            newmatrix(i,j)+=newmatrix(i,j-1);
        }
    }
    
    
    return newmatrix;
}

/* gives a vector in spherical coordinates*/

Spherical3D
anisotropic::pointlookup(matrix<double> &probabilities){
    Functions random;
    double value3 = random.Random(1);
    
    cout<<value3<<endl;
    
    int col =0;
    int row =0;
    
    if(value3<probabilities(0,0)) return Spherical3D(1,0,0);
    
    
    for(int i=0; i<probabilities.get_cols(); i++){
        if(probabilities(i,0)>value3){
            col = i-1;
            break;
        }
        else(col = probabilities.get_cols()-1);
    }
    
    
    for(int i=0; i<probabilities.get_rows(); i++){
        if(probabilities(col,i)>value3){
            row = i-1;
            break;
        }
        else(row = probabilities.get_rows()-1);
    }
    
    
    double r = 1;
    double theta = 0;
    double phi = 0;
    
    theta = (M_PI*col)/probabilities.get_cols();
    phi = (2*M_PI*row)/probabilities.get_rows();
    
    return Spherical3D(r,theta,phi);
}

matrix<double>
anisotropic::genvector(){
    matrix<double> matrix = probs(180, 180, 1000000, 2);
    return cumulative(matrix);
}