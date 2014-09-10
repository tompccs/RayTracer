#include "MATLABPrint.h"

string
MATLABPrint::CentrePoint(Material* box){
    Point3D centre;
    
    Point3D ctop = box->GetTop().Centre();
    Point3D cbase = box->GetBase().Centre();
    
    centre = cbase + ((ctop-cbase)/2);
    
    ostringstream buffer;
    buffer<<"["<<centre.x/100<<","<<centre.y/100<<","<<centre.z/100<<"]";
    return buffer.str();

}

string
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

void
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

string
MATLABPrint::PositionPrint(Photon *photon){
    ostringstream buffer;
    buffer<<"["<<photon->GetMomentum().x<<","<<photon->GetPosition().y<<","<<photon->GetPosition().z<<"]";
    return buffer.str();
}