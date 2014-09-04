#include <iostream>
#include "Test.h"
#include "Functions.h"
#include "FresnelJackson.h"
#include <vector>
#include "MultipleObjects.h"

using namespace std;

void run(double runs,int lscs, int start, int end, bool debug){
    
    //Main algorithm. runs = runs per wavelength. debug = debug mode.
    
    //Creates environment
    
    Sheet* worldbase = new Sheet;
    Material* world = new Material;
    FresnelJackson* inout = new FresnelJackson;
    Functions* calc = new Functions;
    Test* print = new Test;
    MultipleObjects* objects = new MultipleObjects;
    
    Point3D A (0,0,0);
    Point3D B (20,0,0);
    Point3D C (0,20,0);
    double h = 20;
    
    worldbase->Set(A,B,C);
    
    world->SetRefractiveIndex(1);
    world->SetConcentration(0);
    
    world->Set(worldbase, h);
    
    Sheet* lscbase = new Sheet;
    Material* lsc = new Material;
    
    Sheet* lscbase2 = new Sheet;
    Material* lsc2 = new Material;
    
    lsc->ReadData(1);
    lsc2->ReadData(1);
    
    Point3D D (1,1,0.01);
    Point3D E (11,1,0.01);
    Point3D F (1,11,0.01);
    double h2 = 0.5;
    
    Point3D J (1,1,0.61);
    Point3D K (11,1,0.61);
    Point3D L (1,11,0.61);
    double h3 = 0.5;
    
    Point3D G (1,1,9);
    Point3D H (11,1,9);
    Point3D I (1,11,9);
    
    Sheet* source = new Sheet;
    
    source->Set(G,H,I);
    
    lscbase->Set(D,E,F);
    lsc->Set(lscbase,h2);
    
    lscbase2->Set(J,K,L);
    lsc2->Set(lscbase2,h3);
    
    lsc->SetRefractiveIndex(1.495);
    lsc->SetConcentration(1e-5);
    
    lsc2->SetRefractiveIndex(1.495);
    lsc2->SetConcentration(1e-5);
    
    objects->StoreWorld(world);
    if(lscs>=1)objects->StoreMaterial(lsc);
    if(lscs==2) objects->StoreMaterial(lsc2);
    
    double hits = 0;
    double photons = 0;
    
    vector<double> output;
    vector<double> inside;
    
    vector<double> d_zero;
    vector<double> d_one;
    vector<double> d_two;
    vector<double> d_three;
    vector<double> d_four;
    vector<double> d_five;
    vector<double> d_morethanfive;
    
    vector<double> QYLossData;
    vector<double> ExitData;
    
    vector<double> Reflected;
    vector<double> NotAbsorbedInside;
    vector<double> InsideAbsorbedExit;
    
    //Loop for each wavelength. Set wavelength Range here.
    
    for(int wavelength = start; wavelength<= end; wavelength++){
        
        double thisphotons = 0;
        double thishits = 0;
        double thisinside = 0;
        double QYLoss = 0;
        double Exit = 0;
        double zero = 0;
        double one = 0;
        double two = 0;
        double three = 0;
        double four = 0;
        double five = 0;
        double morethanfive = 0;
        double reflected = 0;
        double notabsorbedinside = 0;
        double insideabsorbedexit = 0;
        
        //Loop for individual wavelength.
        
        for(int i = 0; i<runs; i++){
            
            Photon* photon = new Photon;
            
            photons++;
            thisphotons++;
            
            double sx, sy;
            
            sx = lscbase->GetA().x + calc->Random(1) * lscbase->GetABLength();
            sy = lscbase->GetB().y + calc->Random(1) * lscbase->GetACLength();
            
            photon->SetPosition(Point3D(sx,sy,9));
            photon->SetMomentum(Vector3D(0,0,-1));
            photon->SetWavelength(wavelength);
            photon->SetRandomPolarisation();
            
            world->CorrectPhotonInside(photon);
            
            if(debug) {
                cout<<"New photon:"<<endl;
                print->PhotonPrint(photon);
            }
            
            while(world->ReturnPhotonInside() & photon->PhotonAliveCheck()){ //While photon is inside world and alive.
                if(!objects->PhotonInMaterial()){ //If photon is not in a LSC.
                    if(world->GetInterfaceDistance(photon)<objects->NextInterfaceDistance(photon)){ //If next boundary is exit.
                        photon->PhotonKill();
                        world->SetPhotonInside(0);
                        if(debug) {
                            cout<<"World exit."<<endl;
                        }
                    }
                    
                    else{
                        inout->In(photon, world, objects->NextInterfaceMaterial(photon), debug); //Else, entrance reflect/refract event.
                    }
                }
                
                
                else while(objects->PhotonInMaterial()&&photon->PhotonAliveCheck()){ //while photon is in LSC.
                    
                    if(photon->GetAbsorbLength()<=objects->CurrentMaterial()->GetInterfaceDistance(photon)){ //If absorption = next event.
                        objects->CurrentMaterial()->AbsorptionEvent(photon,debug); //Absorption event.
                        
                    }
                    
                    else{ //If boundary is next event.
                        
                        if(!(objects->CurrentMaterial()->GetInterfaceSheet(photon) == objects->CurrentMaterial()->GetBase()) & !(objects->CurrentMaterial()->GetInterfaceSheet(photon) == objects->CurrentMaterial()->GetTop())){
                            photon->PhotonKill(); //If sheet is not top or bottom. Kill photon + add counters.
                            hits++;
                            thishits++;
                            if(debug) cout<<"Hit."<<endl;
                        }
                        else{
                            inout->Out(photon, objects->NextInterfaceMaterial(photon), objects->CurrentMaterial(), debug, objects); //Otherwise exit reflect/refract event.
                        }
                    }
                }
            }
            
            //Counters
            
            if(photon->GetInside()) thisinside++;
            if(photon->GetAbsorptions()==0) zero++;
            if(photon->GetAbsorptions()==1) one++;
            if(photon->GetAbsorptions()==2) two++;
            if(photon->GetAbsorptions()==3) three++;
            if(photon->GetAbsorptions()==4) four++;
            if(photon->GetAbsorptions()==5) five++;
            if(photon->GetAbsorptions()>5) morethanfive++;
            if(photon->GetQYLoss()==1) QYLoss++;
            if(photon->GetExit()==1) Exit++;
            if(!photon->GetInside()) reflected++;
            if(photon->GetAbsorptions()==0 && photon->GetInside()) notabsorbedinside++;
            if(photon->GetInside()&&photon->GetAbsorptions()!=0&&photon->GetExit()) insideabsorbedexit++;

            
            //Deletes photon.
            
            world->SetPhotonInside(0);
            objects->ResetPhotonsInside();
            delete photon;
            
        }
        
        //Adds value for individual wavelengths to vector.
        
        inside.push_back(100*thisinside/thisphotons);
        output.push_back(100*thishits/thisphotons);
        d_zero.push_back(100*zero/thisphotons);
        d_one.push_back(100*one/thisphotons);
        d_two.push_back(100*two/thisphotons);
        d_three.push_back(100*three/thisphotons);
        d_four.push_back(100*four/thisphotons);
        d_five.push_back(100*five/thisphotons);
        d_morethanfive.push_back(100*morethanfive/thisphotons);
        QYLossData.push_back(100*QYLoss/thisphotons);
        ExitData.push_back(100*Exit/thisphotons);
        Reflected.push_back(100*reflected/thisphotons);
        NotAbsorbedInside.push_back(100*notabsorbedinside/thisphotons);
        InsideAbsorbedExit.push_back(100*insideabsorbedexit/thisphotons);
        
    }
    
    //Prints vectors to files for individual wavelengths.
    
    print->PrintVectorFile(output, "output.txt");
    print->PrintVectorFile(inside, "inside.txt");
    print->PrintVectorFile(d_zero, "0.txt");
    print->PrintVectorFile(d_one, "1.txt");
    print->PrintVectorFile(d_two, "2.txt");
    print->PrintVectorFile(d_three, "3.txt");
    print->PrintVectorFile(d_four, "4.txt");
    print->PrintVectorFile(d_five, "5.txt");
    print->PrintVectorFile(d_morethanfive, "morethan5.txt");
    print->PrintVectorFile(QYLossData, "qyloss.txt");
    print->PrintVectorFile(ExitData, "exit.txt");
    print->PrintVectorFile(Reflected, "reflected.txt");
    print->PrintVectorFile(NotAbsorbedInside, "nai.txt");
    print->PrintVectorFile(InsideAbsorbedExit, "iae.txt");
    
    //Calculates total efficiency and prints as 'result'
    
    double result = 100 * hits/photons;
    vector<double> finalresult;
    finalresult.push_back(result);
    print->PrintVectorFile(finalresult,"efficiency.txt");
    
    cout<<"Optical Efficiency: "<<result<<"%"<<endl;
    
}

int main(int argc, const char * argv[]){    
    run(10000,2,350,520,0);
}
