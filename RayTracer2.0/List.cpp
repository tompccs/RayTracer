#include "List.h"

vector <double>
List::read(string filename){
    fstream file;
	vector <double> list; // 1D Vector to store list.
    
	// Read file
	file.open(filename, ios::in); // Open file
	if (file.is_open()) { // If file has correctly opened...
		// Dynamically store data into array
		while (file.good()) { // ... and while there are no errors,
            double newdata;
            file >> newdata;
			list.push_back(newdata); // add a new row,
        }
    }
	else cout << "Unable to open file." << endl;
	file.close();
    return list;
}

vector <double>
List::auconvert(vector <double> &input){
    vector <double> out = input;
    for(int i=1; i<out.size(); i++){
        out[i] = out[i] + out[i-1];
    }
    for(int i=0; i<out.size(); i++){
        out[i] = out[i] / out[out.size()-1];
    }
    return out;
}

vector <double> //Input Old X vector Y vector, and new X vector, returns Y vector.
List::interp1(vector< double > &x, vector< double > &y, vector< double > &x_new){
    vector< double > y_new;
    y_new.reserve( x_new.size() );
    
    std::vector< double > dx, dy, slope, intercept;
    dx.reserve( x.size() );
    dy.reserve( x.size() );
    slope.reserve( x.size() );
    intercept.reserve( x.size() );
    for( int i = 0; i < x.size(); ++i ){
        if( i < x.size()-1 )
        {
            dx.push_back( x[i+1] - x[i] );
            dy.push_back( y[i+1] - y[i] );
            slope.push_back( dy[i] / dx[i] );
            intercept.push_back( y[i] - x[i] * slope[i] );
        }
        else
        {
            dx.push_back( dx[i-1] );
            dy.push_back( dy[i-1] );
            slope.push_back( slope[i-1] );
            intercept.push_back( intercept[i-1] );
        }
    }
    
    for ( int i = 0; i < x_new.size(); ++i )
    {
        int idx = findNearestNeighbourIndex( x_new[i], x );
        y_new.push_back( slope[idx] * x_new[i] + intercept[idx] );
        
    }
    return y_new;
}

int
List::findNearestNeighbourIndex(double value, vector<double> &x){
    double dist = DBL_MAX;
    int idx = -1;
    for ( int i = 0; i < x.size(); ++i ) {
        double newDist = value - x[i];
        if ( newDist > 0 && newDist < dist ) {
            dist = newDist;
            idx = i;
        }
    }
    
    return idx;
}

double
List::findFirstIndexLower(double &value, vector<double> &ydata){
    for(int i=0; i<ydata.size(); i++){
        if(ydata[i]>=value){
            return i;
        }
    }
    return ydata.size()-1;
}

double
List::GetAfromB(double yvalue, vector<double> &xdata, vector<double> &ydata){
    double indexlower = findFirstIndexLower(yvalue, ydata);
    double dx = xdata[indexlower+1]-xdata[indexlower];
    double factor = (yvalue-ydata[indexlower])/(ydata[indexlower+1]+ydata[indexlower]);
    return xdata[indexlower] + (factor * dx);
}

vector<double>
List::newxdata(vector<double> &xdata){
    vector<double> data;
    double max = xdata.size()-1;
    double first = xdata[0];
    double scale = (xdata[max]-xdata[0])/(xdata.size());
    for(int i=0; i<xdata.size(); i++){
        double value = first + i*scale;
        data.push_back(value);
    }
    return data;
};