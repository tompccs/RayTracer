#include "MultipleObjects.h"

void
MultipleObjects::StoreMaterial(Material* object){ //stores a material
    object->SetMaterialNumber(ObjectList.size()+1);
    ObjectList.push_back(object);
    
}

void //stores the world dimensions
MultipleObjects::StoreWorld(Material *WORLD){
    world = WORLD;
}

void //prints the size of the object
MultipleObjects::PrintSize(){
    cout<<ObjectList.size()<<endl;
}

Material*& //returns the current material.
MultipleObjects::CurrentMaterial(){
    for(int i = 0; i<ObjectList.size(); i++){
        if(ObjectList[i]->ReturnPhotonInside())
            return ObjectList[i];
    }
    return world;
}

bool //returns whether the photon is currently inside a material
MultipleObjects::PhotonInMaterial(){
    for(int i = 0; i<ObjectList.size(); i++){
        if(ObjectList[i]->ReturnPhotonInside())
            return 1;
    }
    return 0;
}

double //returns the distance to the next interface
MultipleObjects::NextInterfaceDistance(Photon* photon){
    vector<double> distances;
    for(int i = 0; i<ObjectList.size(); i++){
        distances.push_back(ObjectList[i]->GetInterfaceDistance(photon));
    }
    return calc.GetVectorMin(distances);
}

Material*& //returns the interface of the next material
MultipleObjects::NextInterfaceMaterial(Photon *photon){
    for(int i = 0; i<ObjectList.size(); i++){
        if(ObjectList[i]->GetInterfaceDistance(photon)==NextInterfaceDistance(photon) && ObjectList[i]->ReturnPhotonInside()==0)
            return ObjectList[i];
    }
    return world;
}

void //resets photon inside to 0 for all
MultipleObjects::ResetPhotonsInside(){
    for(int i = 0; i<ObjectList.size(); i++){
        ObjectList[i]->SetPhotonInside(0);
    }
}

vector<Material*> //table of materials
MultipleObjects::GetObjectList(){
    return ObjectList;
}