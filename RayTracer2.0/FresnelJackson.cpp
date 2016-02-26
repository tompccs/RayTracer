#include "FresnelJackson.hpp"

/*void
FresnelJackson::Calculate(Vector3D &OldMomentum, Vector3D &OldPolarisation, Vector3D &theFacetNormal, double &Rindex1, double &Rindex2, bool debug){
    
    double kCarTolerance = 1e-8;
    
    double sini, sinr, cosr, E1_perp, E1_parl, s1, E2_perp, E2_parl, E2_total, s2, TransCoeff, E2_abs, C_parl, C_perp;

    
    double PdotN;
    double EdotN;
    double cosi;
    
    Vector3D A_trans;
    Vector3D A_paral_inc;
    Vector3D A_paral;
    
    double alpha;
    
    
    //Normalise all vectors
    
    OldMomentum.Normalise();
    OldPolarisation.Normalise();
    theFacetNormal.Normalise();
    
    if(debug){
        if(fabs(Dot(OldMomentum, OldPolarisation)>kCarTolerance)) {
            cout<< "Oldmomentum and Oldpolarisation are not orthogonal." <<endl;
        }
    }
    
    PdotN = Dot(OldMomentum, theFacetNormal);
    EdotN = Dot(OldPolarisation, theFacetNormal);
    
    cosi = PdotN;
    
    if(fabs(cosi) < 1.0 - kCarTolerance){
        sini = sqrt(1-cosi*cosi);
        sinr = sini * Rindex1/Rindex2;
    }else{
        sini = 0;
        sinr = 0;
    }
    
    if(sinr >= 1){
        //Simulate total internal reflection
        
        NewMomentum = OldMomentum - theFacetNormal*(2*PdotN);
        NewPolarisation = -OldPolarisation + theFacetNormal*(2*EdotN);
        Transmitted = 0;
    }
    
    else{
        if(cosi>0){
            cosr = sqrt(1-sinr*sinr);
        }
        
        if(sini>0){
            A_trans = -Cross(OldMomentum, theFacetNormal);
            A_trans.Normalise();
            A_paral_inc = Cross(OldMomentum, theFacetNormal);
            A_paral_inc.Normalise();
            E1_perp = Dot(OldPolarisation, A_trans);
            E1_parl = Dot(OldPolarisation, A_paral_inc);
        }else{
            A_trans = OldPolarisation;
            E1_perp = 0;
            E1_parl = 0;
        }
        
        s1 = Rindex1*cosi;
        E2_perp = 2*s1*E1_perp/(Rindex1*cosi+Rindex2*cosr);
        E2_parl = 2*s1*E1_parl/(Rindex2*cosi+Rindex1*cosr);
        E2_total = E2_perp*E2_perp + E2_parl*E2_parl;
        s2 = Rindex2*cosr*E2_total;
        
        TransCoeff = s2/s1;
        
        double genrand = calc.Random(1);
        
        cout<<"TransCoeff is: "<<TransCoeff<<" and genrand is: "<<genrand<<endl;
        
        if(genrand>TransCoeff){//Photon is reflected
            
            NewMomentum = OldMomentum - theFacetNormal*(2*PdotN);
            
            if (sini > 0){ //Incident ray oblique
                
                E2_parl = (Rindex2*E2_parl/Rindex1) - E1_parl;
                E2_perp = E2_perp - E1_perp;
                E2_total = E2_perp*E2_perp + E2_parl*E2_parl;
                A_paral = Cross(NewMomentum,A_trans);
                A_paral.Normalise();
                E2_abs = sqrt(E2_total);
                C_parl = E2_parl/E2_abs;
                C_perp = E2_perp/E2_abs;
                
                NewPolarisation = C_parl*A_paral + C_perp*A_trans;
                
            }else{ //Incident ray perpendicular
                
                if(Rindex2>Rindex1){
                    NewPolarisation = - OldPolarisation;
                }
                else{
                    NewPolarisation = OldPolarisation;
                }
                
            }
            
            NewPolarisation = ProjectionOnPlane(NewMomentum, NewPolarisation);
            
            Transmitted = 0;
        }
        else{ //Photon gets Transmitted
            Transmitted = 1;

            if(sini > 0.0){ //Incident ray oblique
            
                alpha = cosi - cosr*(Rindex2/Rindex1);
                NewMomentum = OldMomentum - alpha*theFacetNormal;
                NewMomentum.Normalise();
                
                A_paral = Cross(NewMomentum,A_trans);
                A_paral.Normalise();
                E2_abs = sqrt(E2_total);
                C_parl = E2_parl/E2_abs;
                C_perp = E2_perp/E2_abs;
                
                NewPolarisation = C_parl*A_paral + C_perp*A_trans;
                
            }else{ //Incident ray perpendicular
            
                NewMomentum = OldMomentum;
                NewPolarisation = OldPolarisation;
            
            }
            
        }
        
    }
    
    NewMomentum.Normalise();
    NewPolarisation.Normalise();
    
}*/

void //calculates new momentum  vector
FresnelJackson::Calculate(Vector3D &OldMomentum, Vector3D &OldPolarisation, Vector3D &theFacetNormal, double &Rindex1, double &Rindex2, bool debug){
    
    if(debug){
    cout<<"Momentum vector is:"<<endl;
     print.PrintVector(OldMomentum);
     cout<<"Polarisation vector is"<<endl;
     print.PrintVector(OldPolarisation);
     cout<<"Normal vector is"<<endl;
     print.PrintVector(theFacetNormal);
     cout<<"Refractive index 1 is: "<<Rindex1<<" and 2 is: "<<Rindex2<<endl;
    }
    
    double sini, sinr, cosr, E1_perp, E1_parl, s1, E2_perp, E2_parl, E2_total, s2, TransCoeff, E2_abs, C_parl, C_perp;
    
    double kCarTolerance = 1e-3;
    
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
        
        if(debug){
            cout<<"Total internal reflection."<<endl;
        }
        
        NewMomentum = OldMomentum - theFacetNormal*(2*PdotN);
        NewPolarisation = -OldPolarisation + theFacetNormal*(2*EdotN);
        Transmitted = 0;
        if(debug){
            cout<<"Reflected"<<endl;
        }
        
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
        
        if(debug){
          cout<<"Random number is:"<<genrand<<"and Coefficient is: "<<TransCoeff<<endl;  
        }
        
        /*if(TransCoeff <= 0){
            cout<<"Negative coefficient of reflection: "<<TransCoeff<< " Exit now."<<endl;
            //exit(2);
        }*/
        
        if(genrand>TransCoeff){ //Reflection
            
            NewMomentum = OldMomentum - theFacetNormal*(2*PdotN);
            
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
                
                NewMomentum = -OldMomentum;
                if (Rindex2 > Rindex1) NewPolarisation = -OldPolarisation;
                else NewPolarisation =   OldPolarisation;
            }
            
            NewPolarisation = ProjectionOnPlane(NewMomentum, NewPolarisation);
            
            Transmitted = 0;
            if(debug){
                cout<<"Reflected"<<endl;
            }
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
            if(debug){
                cout<<"Transmitted"<<endl;
            }
        }
    }
    
    if(debug){
        cout<<"New Momentum is:"<<endl;
        print.PrintVector(NewMomentum);
    }
}

void //entrance event
FresnelJackson::In(Photon *photon, Material *world, Material *lsc, bool &debug){
    
    Vector3D N = (lsc->GetInterfaceSheet(photon).GetNormal());
    
    Calculate(photon->GetMomentum(), photon->GetPolarisation(), N, world->GetRefractiveIndex(), lsc->GetRefractiveIndex(), debug);
    
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

void //exit event
FresnelJackson::Out(Photon *photon, Material *material2, Material *material1, bool &debug, MultipleObjects* objects){
    Vector3D N = -(material1->GetInterfaceSheet(photon).GetNormal());
    double value = photon->GetAbsorbLength() - material1->GetInterfaceDistance(photon);
    
    Calculate(photon->GetMomentum(), photon->GetPolarisation(), N, material1->GetRefractiveIndex(), material2->GetRefractiveIndex(), debug);
    
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

Vector3D //projects a vector onto a plane
FresnelJackson::ProjectionOnPlane(Vector3D plane_normal, Vector3D vector){
    plane_normal.Normalise();
    return (vector - Dot(vector,plane_normal)*(plane_normal));
}

Vector3D
FresnelJackson::GetNewMomentum(){
    return NewMomentum;
}

Vector3D
FresnelJackson::GetNewPolarisation(){
    return NewPolarisation;
}

void //entrance event
FresnelJackson::CurvedIn(Photon *photon, Material *world, curvedbox *FLSC, bool &debug){
    
    int surface = FLSC->NextInterface(*photon,debug);
    Vector3D N;
    if(surface!=3 && surface!=0){
        N = FLSC->GetNextNormal(*photon, debug);
        N.Normalise();
        
    }
    else{
        N = -FLSC->GetNextNormal(*photon, debug);
        N.Normalise();
    }

    
    Calculate(photon->GetMomentum(), photon->GetPolarisation(), N, world->GetRefractiveIndex(), FLSC->GetRefractiveIndex(), debug);
    
    photon->SetPosition(photon->GetPosition()+photon->GetMomentum()*FLSC->NextInterfaceDistance(*photon,debug));
    photon->SetMomentum(NewMomentum);
    photon->SetPolarisation(NewPolarisation);
    
    if(Transmitted){
        FLSC->SetPhotonInside(1);
        FLSC->SetInitialAbsorbLength(photon);
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
FresnelJackson::CurvedOut(Photon *photon, curvedbox *FLSC, Material *world, bool &debug){
    int surface = FLSC->NextInterface(*photon,debug);
    Vector3D N;
    if(surface!=3){
        N = -FLSC->GetNextNormal(*photon, debug);
        N.Normalise();
        
    }
    else{
        N = FLSC->GetNextNormal(*photon, debug);
        N.Normalise();
    }
    
    
    double value = photon->GetAbsorbLength() - FLSC->NextInterfaceDistance(*photon,debug);
    
    Calculate(photon->GetMomentum(), photon->GetPolarisation(), N, FLSC->GetRefractiveIndex(), world->GetRefractiveIndex(), debug);
    
    photon->SetPosition(photon->GetPosition() + photon->GetMomentum()*FLSC->NextInterfaceDistance(*photon,debug));
    photon->SetMomentum(NewMomentum.Normalise());
    photon->SetPolarisation(NewPolarisation.Normalise());
    
    if(!Transmitted){
        photon->SetAbsorblength(value);
        if(debug){
            cout<<"Exit boundary event. Reflection (No exit):"<<endl;
            print.PhotonPrint(photon);
        }
    }
    
    else{
        FLSC->SetPhotonInside(0);
        photon->SetAbsorblength(DBL_MAX);
        if(debug){
            cout<<"Exit boundary event. Refraction:"<<endl;
            print.PhotonPrint(photon);
        }
    }
    
}

void //entrance event
FresnelJackson::FlexiIn(Photon *photon, Material *world, flexi *FLSC, bool &debug){
    
    Vector3D N;
        N = FLSC->GetNextNormal(*photon);
        N.Normalise();
    
    Calculate(photon->GetMomentum(), photon->GetPolarisation(), N, world->GetRefractiveIndex(), FLSC->GetRefractiveIndex(), debug);
    
    photon->SetPosition(photon->GetPosition()+photon->GetMomentum()*FLSC->IntersectionDistance());
    photon->SetMomentum(NewMomentum);
    photon->SetPolarisation(NewPolarisation);
    
    if(Transmitted){
        bool b = 1;
        FLSC->SetPhotonInside(b);
        FLSC->SetInitialAbsorbLength(photon);
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
FresnelJackson::FlexiOut(Photon *photon, flexi *FLSC, Material *world, bool &debug){
    
    Vector3D N = FLSC->GetNextNormal(*photon);
    N.Normalise();
    
    
    double value = photon->GetAbsorbLength() - FLSC->IntersectionDistance();
    
    Calculate(photon->GetMomentum(), photon->GetPolarisation(), N, FLSC->GetRefractiveIndex(), world->GetRefractiveIndex(), debug);
    
    photon->SetPosition(photon->GetPosition() + photon->GetMomentum()*FLSC->IntersectionDistance());
    photon->SetMomentum(NewMomentum.Normalise());
    photon->SetPolarisation(NewPolarisation.Normalise());
    
    if(!Transmitted){
        photon->SetAbsorblength(value);
        if(debug){
            cout<<"Exit boundary event. Reflection (No exit):"<<endl;
            print.PhotonPrint(photon);
        }
    }
    
    else{
        bool b = 0;
        FLSC->SetPhotonInside(b);
        photon->SetAbsorblength(DBL_MAX);
        if(debug){
            cout<<"Exit boundary event. Refraction:"<<endl;
            print.PhotonPrint(photon);
        }
    }
    
}

void //entrance event
FresnelJackson::NewCurvedIn(Photon *photon, Material *world, curvedlsc& FLSC, bool &debug){
    
    int surface = FLSC.NextIntersection(*photon, debug);
    Vector3D N;
    if(surface!=2 && surface!=0){
        N = FLSC.NextNormal(*photon, debug);
        N.Normalise();
        
    }
    else{
        N = -FLSC.NextNormal(*photon, debug);
        N.Normalise();
    }
    
    
    Calculate(photon->GetMomentum(), photon->GetPolarisation(), N, world->GetRefractiveIndex(), FLSC.GetRefractiveIndex(), debug);
    
    photon->SetPosition(photon->GetPosition()+photon->GetMomentum()*FLSC.NextDistance(*photon, debug));
    photon->SetMomentum(NewMomentum);
    photon->SetPolarisation(NewPolarisation);
    
    if(Transmitted){
        FLSC.SetPhotonInside(1);
        FLSC.SetInitialAbsorbLength(photon);
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
FresnelJackson::NewCurvedOut(Photon *photon, curvedlsc& FLSC, Material *world, bool &debug){
    int surface = FLSC.NextIntersection(*photon, debug);
    Vector3D N;
    if(surface!=3 && surface!=0){
        N = -FLSC.NextNormal(*photon, debug);
        N.Normalise();
        
    }
    else{
        N = FLSC.NextNormal(*photon, debug);
        N.Normalise();
    }
    
    
    double value = photon->GetAbsorbLength() - FLSC.NextDistance(*photon,debug);
    
    Calculate(photon->GetMomentum(), photon->GetPolarisation(), N, FLSC.GetRefractiveIndex(), world->GetRefractiveIndex(), debug);
    
    photon->SetPosition(photon->GetPosition() + photon->GetMomentum()*FLSC.NextDistance(*photon,debug));
    photon->SetMomentum(NewMomentum.Normalise());
    photon->SetPolarisation(NewPolarisation.Normalise());
    
    if(!Transmitted){
        photon->SetAbsorblength(value);
        if(debug){
            cout<<"Exit boundary event. Reflection (No exit):"<<endl;
            print.PhotonPrint(photon);
        }
    }
    
    else{
        FLSC.SetPhotonInside(0);
        photon->SetAbsorblength(DBL_MAX);
        if(debug){
            cout<<"Exit boundary event. Refraction:"<<endl;
            print.PhotonPrint(photon);
        }
    }
    
}