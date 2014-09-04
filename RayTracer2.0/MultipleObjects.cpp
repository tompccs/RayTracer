#include "MultipleObjects.h"

void
MultipleObjects::StoreMaterial(Material* object){
    object->SetMaterialNumber(ObjectList.size()+1);
    ObjectList.push_back(object);
    
}

void
MultipleObjects::StoreWorld(Material *WORLD){
    world = WORLD;
}

void
MultipleObjects::PrintSize(){
    cout<<ObjectList.size()<<endl;
}

Material*&
MultipleObjects::CurrentMaterial(){
    for(int i = 0; i<ObjectList.size(); i++){
        if(ObjectList[i]->ReturnPhotonInside())
            return ObjectList[i];
    }
    return world;
}

bool
MultipleObjects::PhotonInMaterial(){
    for(int i = 0; i<ObjectList.size(); i++){
        if(ObjectList[i]->ReturnPhotonInside())
            return 1;
    }
    return 0;
}

double
MultipleObjects::NextInterfaceDistance(Photon* photon){
    vector<double> distances;
    for(int i = 0; i<ObjectList.size(); i++){
        distances.push_back(ObjectList[i]->GetInterfaceDistance(photon));
    }
    return calc.GetVectorMin(distances);
}

Material*&
MultipleObjects::NextInterfaceMaterial(Photon *photon){
    for(int i = 0; i<ObjectList.size(); i++){
        if(ObjectList[i]->GetInterfaceDistance(photon)==NextInterfaceDistance(photon) && ObjectList[i]->ReturnPhotonInside()==0)
            return ObjectList[i];
    }
    return world;
}

void
MultipleObjects::ResetPhotonsInside(){
    for(int i = 0; i<ObjectList.size(); i++){
        ObjectList[i]->SetPhotonInside(0);
    }
}