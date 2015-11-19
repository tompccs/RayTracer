#include "Material.h"

//'Get' method for process.
Process&
Material::GetProcess(){
    return Events;
}

//'Set' method for process.
void
Material::SetProcess(const Process& n){
    Events = n;
}

//Returns Refractive Index
double&
Material::GetRefractiveIndex(){
    return RefractiveIndex;
}

//Sets Refractive Index
void
Material::SetRefractiveIndex(double n){
    RefractiveIndex = n;
}

//Returns Concentration
double&
Material::GetConcentration(){
    return Concentration;
}

//Sets Concentration
void
Material::SetConcentration(double c){
    Concentration = c;
}

//Reads data from a list, of just numbers
void
Material::ReadData(bool evenspaced){
    Events.ReadData(evenspaced);
}

//Sets absortion length as calculated
void
Material::SetInitialAbsorbLength(Photon* P){
    double i = Events.GetPathLength(P->GetWavelength(), Concentration);
    P->SetAbsorblength(i);
}

//Absorbtion event
void
Material::AbsorptionEvent(Photon *P, bool& debug, bool& matlabprint, vector<Point3D>& dyeabs, vector<Point3D>& photonpath){
    P->Addabsorption(); // Absorb photon
    if(debug){ //Debug lines
        cout<<"Photon absorbed"<<endl;
    }
    
    if(matlabprint){ //Matlab Print Lines
        Point3D newlocation = P->GetPosition()+P->GetMomentum()*P->GetAbsorbLength();
        dyeabs.push_back(newlocation);
        photonpath.push_back(newlocation);
        
    }
    
    if(Events.QuantumYieldCheck(P->GetWavelength())){ //If QY = 1
        Vector3D a; //Reemit particle
        P->SetPosition(P->GetPosition()+P->GetMomentum()*P->GetAbsorbLength());
        P->SetMomentum(a.GetRandomUnitVector());
        P->SetWavelength(Events.GetEmissionWavelength());
        P->SetRandomPolarisation();
        SetInitialAbsorbLength(P);
        
        if(debug){
            cout<<"and reemitted."<<endl;
            print.PhotonPrint(P);
        }
        
    }
    else{
        if(debug) cout<<"but not reemitted."<<endl<<endl;
        P->PhotonKill(); //Or if 0, kill photon.
        P->SetQYLoss();
    }
}

//Indicator that Material has a photon inside
void
Material::SetPhotonInside(bool inside){
    PhotonInside = inside;
}

//Returns PhotonInside Indicator
bool
Material::ReturnPhotonInside(){
    return PhotonInside;
}

//Checks that indicator and reality match
void
Material::CorrectPhotonInside(Photon *photon){
    PhotonInside = PointinBox(photon);
}

//Sets the material number
void
Material::SetMaterialNumber(long n){
    Materialnumber = n;
}

//Returns the material number
long&
Material::GetMaterialNumber(){
    return Materialnumber;
}

void
Material::SetCurved(bool yesno){
    Curved = yesno;
}

bool&
Material::GetCurved(){
    return Curved;
}