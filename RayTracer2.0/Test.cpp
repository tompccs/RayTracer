#include "Test.h"

void
Test::PrintPoint(const Point3D& point){
    cout<<"(";
    cout<<point.x;
    cout<<",";
    cout<<point.y;
    cout<<",";
    cout<<point.z;
    cout<<")";
    cout<<endl;
}

void
Test::PrintVector(const Vector3D& vector){
    cout<<"(";
    cout<<vector.x;
    cout<<",";
    cout<<vector.y;
    cout<<",";
    cout<<vector.z;
    cout<<")";
    cout<<endl;
}

void
Test::PrintBool(const bool &b){
    if(b) cout<<"True."<<endl;
    else if(!b) cout<<"False."<<endl;
}

void
Test::PrintSheet(Sheet &sheet){
    PrintPoint(sheet.GetA());
    PrintPoint(sheet.GetB());
    PrintPoint(sheet.GetC());
    
}

void
Test::PrintVectorFile(vector<double> list, string filename){
    ofstream myfile (filename);
    if (myfile.is_open())
    {
        for(int i=0; i<list.size(); i++){
            myfile << list[i]<<endl;
        }
        myfile.close();
    }
    else cout << "Unable to open file";
}

void
Test::PhotonPrint(Photon *photon){
    cout<<"Photon has position: ";
    PrintPoint(photon->GetPosition());
    cout<<" and momentum";
    PrintVector(photon->GetMomentum());
    cout<<endl;
    cout<<"Photon has wavelength: ";
    cout<<photon->GetWavelength()<<"nm"<<endl;
    cout<<endl;
    cout<<"Distance to absorption: "<<photon->GetAbsorbLength()<<"cm"<<endl;
    cout<<endl;
}