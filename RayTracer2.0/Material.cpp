#include "Material.h"

Process&
Material::GetProcess(){
    return Events;
}

void
Material::SetProcess(const Process& n){
    Events = n;
}

double&
Material::GetRefractiveIndex(){
    return RefractiveIndex;
}

void
Material::SetRefractiveIndex(double n){
    RefractiveIndex = n;
}

double&
Material::GetConcentration(){
    return Concentration;
}

void
Material::SetConcentration(double c){
    Concentration = c;
}

void
Material::ReadData(bool evenspaced){
    Events.ReadData(evenspaced);
}

void
Material::SetInitialAbsorbLength(Photon* P){
    double i = Events.GetPathLength(P->GetWavelength(), Concentration);
    P->SetAbsorblength(i);
}

void
Material::AbsorptionEvent(Photon *P, bool& debug){
    P->Addabsorption();
    if(debug){
        cout<<"Photon absorbed"<<endl;
        print.PhotonPrint(P);
    }
    
    
    if(Events.QuantumYieldCheck(P->GetWavelength())){
        Vector3D a;
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
        P->PhotonKill();
        P->SetQYLoss();
    }
}

void
Material::SetPhotonInside(bool inside){
    PhotonInside = inside;
}

bool
Material::ReturnPhotonInside(){
    return PhotonInside;
}

void
Material::CorrectPhotonInside(Photon *photon){
    PhotonInside = PointinBox(photon);
}
