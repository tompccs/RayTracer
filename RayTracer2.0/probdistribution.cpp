#include "probdistribution.hpp"

void
probdistribution::AddColumn(vector<double> NewColomn){
    values.push_back(NewColomn);
}

void
probdistribution::AddColumnFromFile(string Filename){
    //cout<<Filename<<endl;
    AddColumn(data.read(Filename));
}

string
probdistribution::FileLocation(double lambda, double radius, string backfront, double theta, bool hot){
    ostringstream oss;
    if(hot){
    oss << "data/hybrid/hotr"<<radius<<"/r"<<radius<<"/l"<<lambda<<"/r"<<radius<<"_l"<<lambda<<"_"<<backfront<<"theta"<<theta<<".txt";
    }else{
    oss << "data/hybrid/coldr"<<radius<<"/r"<<radius<<"/l"<<lambda<<"/r"<<radius<<"_l"<<lambda<<"_"<<backfront<<"theta"<<theta<<".txt";

    }
    return oss.str();
}

string
probdistribution::FrontBackFile(double lambda, double radius, bool hot){
    ostringstream oss;
    if(hot){
    oss << "data/hybrid/hotr"<<radius<<"/r"<<radius<<"/l"<<lambda<<"/r"<<radius<<"_l"<<lambda<<"_ratio.txt";
    }else{
    oss << "data/hybrid/coldr"<<radius<<"/r"<<radius<<"/l"<<lambda<<"/r"<<radius<<"_l"<<lambda<<"_ratio.txt";

    }
    return oss.str();
}

void
probdistribution::StoreValues(double lambda, double radius, string frontback, double startangle, double endangle, bool hot){
    
    for(double i=startangle; i<=endangle; i++){
        string a = FileLocation(lambda, radius, frontback, i, hot);
        AddColumnFromFile(a);
    }
}

void
probdistribution::PrintValue(int x, int y){
    cout<<values[x][y];
}

double&
probdistribution::GetValue(int x, int y){
    return values[x][y];
}

void
probdistribution::SetValue(int x, int y, double value){
    values[x][y] = value;
}

void
probdistribution::MakeCumulativeProbability(){
    //First divide all values of theta = 0 by 360 to remove degeneracy.
    for(int i=0; i<values[0].size(); i++){
        SetValue(0, i, GetValue(0, i)/values[0].size());
    }
    
    //Then make each value addition of the sum of the previous
    
    double cumulval = 0;
    for(int x=0; x<values.size(); x++){
        for(int y=0; y<values[x].size(); y++){
            cumulval = cumulval + values[x][y];
            values[x][y] = cumulval;
            
        }
    }
    
    //Divide by max value to normalise probability to one!
    
    for(int x=0; x<values.size(); x++){
        for(int y=0; y<values[x].size(); y++){
            values[x][y] = values[x][y]/cumulval;
        }
    }
}

int
probdistribution::GetTheta(double &Prob){
    int x = 0;
    //cout<<Prob;
    while (Prob > values[x][360]) {
        //cout<<values[x][360]<<endl;
        x++;
    }
    
    return x;
}

int
probdistribution::GetPhi(double &Prob, int Theta){
    int y = 0;
    while (Prob > values[Theta][y]){
        y++;
    }
    
    return y+1;
}