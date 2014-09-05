#include "FresnelJackson.h"

void
FresnelJackson::Calculate(Vector3D &OldMomentum, Vector3D &OldPolarisation, Vector3D &theFacetNormal, double &Rindex1, double &Rindex2){
    
    double sini, sinr, cosr, E1_perp, E1_parl, s1, E2_perp, E2_parl, E2_total, s2, TransCoeff, E2_abs, C_parl, C_perp;
    
    double kCarTolerance = 1e-8;
    
    Vector3D A_trans, A_paral_inc, A_paral;
    
    double PdotN = Dot(OldMomentum,theFacetNormal);
    double EdotN = Dot(OldPolarisation,theFacetNormal);
    
    double cosi = PdotN;
    
    if(abs(cosi) < 1.0 - kCarTolerance){
        
         sini = sqrt(1.-cosi*cosi);
         sinr = sini*Rindex1/Rindex2;
    }
    else{
             sini = 0.0;
             sinr = 0.0;
    }

    if(sinr >= 1.0){ //Total Internal Reflection
        
        NewMomentum = OldMomentum - theFacetNormal*(2*PdotN);
        NewPolarisation = -OldPolarisation + theFacetNormal*(2*EdotN);
        Transmitted = 0;
        
    }
    else{
        
        if(cosi > 0.0)
           cosr =  sqrt(1.-sinr*sinr);
        if (sini > 0.0){
            A_trans = -Cross(OldMomentum,theFacetNormal).Normalise();
            A_paral_inc = Cross(OldMomentum,A_trans).Normalise();
            E1_perp = Dot(OldPolarisation,A_trans);
            E1_parl = Dot(OldPolarisation,A_paral_inc);
        }
        
        else{
            A_trans  = OldPolarisation;
            E1_perp  = 0.0;
            E1_parl  = 1.0;
        }
        
        s1 = Rindex1*cosi;
        E2_perp = 2.*s1*E1_perp/(Rindex1*cosi+Rindex2*cosr);
        E2_parl = 2.*s1*E1_parl/(Rindex2*cosi+Rindex1*cosr);
        E2_total = E2_perp*E2_perp + E2_parl*E2_parl;
        s2 = Rindex2*cosr*E2_total;
        
        TransCoeff = s2/s1;
        
        double genrand = calc.Random(1);
        
        if(genrand>TransCoeff){ //Reflection
            
            if (sini > 0.0){
                E2_parl   = Rindex2*E2_parl/Rindex1 - E1_parl;
                E2_perp   = E2_perp - E1_perp;
                E2_total  = E2_perp*E2_perp + E2_parl*E2_parl;
                A_paral   = Cross(NewMomentum,A_trans).Normalise();
                E2_abs    = sqrt(E2_total);
                C_parl    = E2_parl/E2_abs;
                C_perp    = E2_perp/E2_abs;
            
            NewPolarisation = C_parl*A_paral + C_perp*A_trans;
                
            }
            
            else{
                    if (Rindex2 > Rindex1) NewPolarisation = -OldPolarisation;
                    else NewPolarisation =   OldPolarisation;
                }
            
            NewPolarisation = ProjectionOnPlane(NewMomentum, NewPolarisation);
        
            Transmitted = 0;
        }
        
        else{
            
            if(sini > 0.0){
                
            double alpha = cosi - cosr*(Rindex2/Rindex1);
            NewMomentum = OldMomentum - alpha*theFacetNormal.Normalise();
            
            A_paral    = Cross(NewMomentum,A_trans).Normalise();
            E2_abs     = sqrt(E2_total);
            C_parl     = E2_parl/E2_abs;
            C_perp     = E2_perp/E2_abs;
            
            NewPolarisation = C_parl*A_paral + C_perp*A_trans;
            
            }
            else{
                NewMomentum = OldMomentum;
                NewPolarisation = OldPolarisation;
            }
  
                Transmitted = 1;
        }
    }
}

void
FresnelJackson::In(Photon *photon, Material *world, Material *lsc, bool &debug){
    
    Vector3D N = (lsc->GetInterfaceSheet(photon).GetNormal());
    
    Calculate(photon->GetMomentum(), photon->GetPolarisation(), N, world->GetRefractiveIndex(), lsc->GetRefractiveIndex());
    
    photon->SetPosition(lsc->GetInterfacePoint(photon));
    photon->SetMomentum(NewMomentum);
    photon->SetPolarisation(NewPolarisation);

    if(Transmitted){
        lsc->SetPhotonInside(1);
        lsc->SetInitialAbsorbLength(photon);
        photon->SetInside();
        
        if(debug){
            cout<<"Entrance boundary event. Refraction:"<<endl;
            print.PhotonPrint(photon);
        }
    }
    
    else{
        if(debug){
            cout<<"Entrance boundary event. Reflection (No entrance):"<<endl;
            print.PhotonPrint(photon);
        }
    }
}

void
FresnelJackson::Out(Photon *photon, Material *material2, Material *material1, bool &debug, MultipleObjects* objects){
    Vector3D N = -(material1->GetInterfaceSheet(photon).GetNormal());
    double value = photon->GetAbsorbLength() - material1->GetInterfaceDistance(photon);

    Calculate(photon->GetMomentum(), photon->GetPolarisation(), N, material1->GetRefractiveIndex(), material2->GetRefractiveIndex());

    photon->SetPosition(material1->GetInterfacePoint(photon));
    photon->SetMomentum(NewMomentum);
    photon->SetPolarisation(NewPolarisation);
    
    if(!Transmitted){
        photon->SetAbsorblength(value);
        if(debug){
            cout<<"Exit boundary event. Reflection (No exit):"<<endl;
            print.PhotonPrint(photon);
        }
    }
    
    else{
        material1->SetPhotonInside(0);
        photon->SetExit();
        material2->SetPhotonInside(1);
        if(objects->PhotonInMaterial()) material2->SetInitialAbsorbLength(photon);
        else photon->SetAbsorblength(DBL_MAX);
        if(debug){
            cout<<"Exit boundary event. Refraction:"<<endl;
            print.PhotonPrint(photon);
        }
    }
}

Vector3D
FresnelJackson::ProjectionOnPlane(Vector3D plane_normal, Vector3D vector){
    plane_normal.Normalise();
    return (vector - Dot(vector,plane_normal)*(plane_normal));
}