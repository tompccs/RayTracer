#include "MATLABPrint.h"

string //returns string '[x,y,z]' for centre point of box
MATLABPrint::CentrePoint(Material* box){
    Point3D centre;
    
    Point3D ctop = box->GetTop().Centre();
    Point3D cbase = box->GetBase().Centre();
    
    centre = cbase + ((ctop-cbase)/2);
    
    ostringstream buffer;
    buffer<<"["<<centre.x/100<<","<<centre.y/100<<","<<centre.z/100<<"]";
    return buffer.str();

}

string //returns string '[x,y,z]' for size of box
MATLABPrint::Size(Material* box){
    Vector3D size;
    
    Point3D A = box->GetBase().GetA();
    Point3D B = box->GetTop().GetB();
    
    size.x = abs(B.x - A.x);
    size.y = abs(B.y - A.y);
    size.z = abs(B.z - A.z);
    
    ostringstream buffer;
    buffer<<"["<<size.x/100<<","<<size.y/100<<","<<size.z/100<<"]";
    return buffer.str();
}

void //prints file with centrepoints & sizes
MATLABPrint::PrintLSCs(MultipleObjects* mo){
    
    vector<Material*> mats = mo->GetObjectList();
    
    vector<string> CentrePoints;
    vector<string> Sizes;
    
    for(int i = 0; i<mats.size(); i++){
        CentrePoints.push_back(CentrePoint(mats[i]));
        Sizes.push_back(Size(mats[i]));
    }
    
    print->PrintTextVector(CentrePoints, "CentrePoints.txt");
    print->PrintTextVector(Sizes, "Sizes.txt");
    
}

string //returns string position of photon
MATLABPrint::PositionPrint(Photon *photon){
    ostringstream buffer;
    buffer<<"["<<photon->GetPosition().x<<","<<photon->GetPosition().y<<","<<photon->GetPosition().z<<"]";
    return buffer.str();
}

void //stores and prints dye absorb positions
MATLABPrint::DyeAbsorbPrint(vector<Point3D> points){
    vector<double> x;
    vector<double> y;
    vector<double> z;
    
    for(int i=0; i<points.size(); i++){
        if(points[i].x >0 && points[i].y >0 && points[i].z >0){
        x.push_back(points[i].x);
        y.push_back(points[i].y);
        z.push_back(points[i].z);
        }
    }
    
    print->PrintVectorFile(x, "dyeabsorb_x.txt");
    print->PrintVectorFile(y, "dyeabsorb_y.txt");
    print->PrintVectorFile(z, "dyeabsorb_z.txt");

}

void //prints path of photon into files
MATLABPrint::PhotonPathPrint(vector<vector<Point3D>>& paths){
    for(int i=0; i<paths.size(); i++){
        vector<double> x;
        vector<double> y;
        vector<double> z;
        
        for(int j=0; i<paths[j].size(); j++){
            if(paths[i][j].x >0 && paths[i][j].y >0 && paths[i][j].z >0){
                x.push_back(paths[i][j].x);
                y.push_back(paths[i][j].y);
                z.push_back(paths[i][j].z);
            }
        }
        
        ostringstream bufferx;
        bufferx<<"photon_"<<i+1<<"_x"<<".txt";
        string filenamex = bufferx.str();
        print->PrintVectorFile(x, filenamex);
        
        ostringstream buffery;
        buffery<<"photon_"<<i+1<<"_y"<<".txt";
        string filenamey = buffery.str();
        print->PrintVectorFile(y, filenamey);
        
        ostringstream bufferz;
        bufferz<<"photon_"<<i+1<<"_z"<<".txt";
        string filenamez = bufferz.str();
        print->PrintVectorFile(z, filenamez);
    }
}