#include "scattering.hpp"

void
scattering::GetDistributions(int radius, int lambda, bool hot){
    
    //Read data and store values for 'front' halfspace.
    front.StoreValues(lambda, radius, "f", 0, 90, hot);
    front.MakeCumulativeProbability();
    
    //Read data and store values for 'back' halfspace.
    back.StoreValues(lambda, radius, "b", 0, 90, hot);
    back.MakeCumulativeProbability();
    
    frontbackdistribution = data.read(front.FrontBackFile(lambda, radius, hot));
}

bool
scattering::RollHalfSpace(){ //Returns 1 for forward, or 0 for back
    double rand = num.Random(1);
    double split = 1/(frontbackdistribution[0]+1);
    //double split = 1/(9.21004+1);
    //cout<<rand<<" and split "<<split<<endl;
    if(rand > split){
        return 1;
    }
    return 0;
}

void
scattering::RollCoordinates(){
    bool half = RollHalfSpace();
    theta = 0;
    phi = 0;
    
    double prob = num.Random(1);
    
    if(half){
        theta = front.GetTheta(prob);
        phi = front.GetPhi(prob, theta);
    }
    
    if(!half){
        theta = back.GetTheta(prob);
        phi = back.GetPhi(prob, theta);
        
        theta = 180-theta;
        phi = 180-phi;
    }
    
    //cout<<"("<<theta<<","<<phi<<") degrees"<<endl;
    
    theta = theta * M_PI / 180;
    phi = phi * M_PI / 180;
    
    //cout<<"("<<theta<<","<<phi<<") radians"<<endl;

}


Vector3D
scattering::Rotated(Vector3D mom, double theta, double phi){
    return mom.RotateAboutZ(phi).RotateAboutX(theta);
}

double&
scattering::GetPhi(){
    return phi;
}

double&
scattering::GetTheta(){
    return theta;
}