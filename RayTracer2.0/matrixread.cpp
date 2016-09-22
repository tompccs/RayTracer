#include "matrixread.hpp"


//Imports matrix and stores as vector of strings to vector<string> text

void
matrixread::Import(const char* filename){
    string line;
    ifstream myfile (filename);
    //int rows = 1;
    if (myfile.is_open())
    {
        while ( getline (myfile,line,'\n') )
        {
            text.push_back(line);
            //cout<<line<<"End of line"<<endl;
        }
        myfile.close();
    }
    
    else cout << "Unable to open file";
    
}

//takes input of a string, and returns a vector<double>

vector<double>
matrixread::Push(string& line){
    vector<double> numbers;
    
    stringstream lineStream(line);
    
    double num;
    
    while (lineStream >> num) numbers.push_back(num);
    
    return numbers;
}


//Takes vectors from <vector>text to <vector<vector>double
vector<vector<double>>&
matrixread::Split(){
    
    int i =0;
    
    int size = (int) text.size();
    
    while(i<size){
        vals.push_back(Push(text[i]));
        i++;
    }
    
    return vals;
}

//Combine Import and split
void
matrixread::Data(const char* filename){
    Import(filename);
    Split();
}

double&
matrixread::GetValue(int i, int j){
    return vals[i][j];
}

void
matrixread::SetValue(int i, int j, double value){
    vals[i][j] = value;
}

void
matrixread::PrintVals(){
    int I = (int) vals.size();
    int J = (int) vals[0].size();
    
    for(int i=0; i<I; i++){
        for(int j=0; j<J; j++){
            cout<<vals[i][j];
            cout<<" ";
        }
        cout<<endl;
    }
}

vector<vector<double>>&
matrixread::GetMatrix(){
    return vals;
}

double&
matrixread::Lookup(double &x){
    
    int i = 0;
    
    while(GetValue(i, 0)<x){
        i++;
    }
    
    return GetValue(i, 1);
}

