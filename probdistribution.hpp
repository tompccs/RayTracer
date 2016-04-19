#ifndef probdistribution_hpp
#define probdistribution_hpp

#include <stdio.h>
#include "List.h"
#include <string>
#include <sstream>

class probdistribution{
    List data;
    vector<vector<double>> values;
    
public:
    
    void AddColumn(vector<double> NewColomn);
    void AddColumnFromFile(string Filename);
    string FileLocation(double lambda, double radius, string backfront, double theta, bool hot);
    string FrontBackFile(double lambda, double radius, bool hot);
    void StoreValues(double lambda, double radius, string frontback, double startangle, double endangle, bool hot);
    
    void PrintValue(int x, int y);
    double& GetValue(int x, int y);
    void SetValue(int x, int y, double value);
    void MakeCumulativeProbability();
    int GetTheta(double& Prob);
    int GetPhi(double& Prob, int Theta);
    
};

#endif /* probdistribution_hpp */
