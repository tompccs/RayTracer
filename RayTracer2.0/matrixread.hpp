#ifndef matrixread_hpp
#define matrixread_hpp

#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdio.h>

using namespace std;

class matrixread{
    vector<vector<double>> vals;
    vector<string> text;
    int rows;
    int cols;

    
public:
    void Import(const char* filename);
    vector<double> Push(string& line);
    vector<vector<double>>& Split();
    
    void Data(const char* filename);

    double& GetValue(int i, int j);
    
    void PrintVals();
    
};


#endif