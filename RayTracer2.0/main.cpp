#include <iostream>
#include "Test.h"
#include "Functions.h"
#include "FresnelJackson.h"
#include <vector>
#include "MultipleObjects.h"
#include "MATLABPrint.h"

using namespace std;

void run(double runs,int lscs, int start, int end, bool debug, bool matlabprint){
    
    //Main algorithm. runs = runs per wavelength. debug = debug mode.
    
    //Creates environment
    
    Material* world = new Material; //Creates new world box.
    
    FresnelJackson* inout = new FresnelJackson; //Calculation for boundarys
    Functions* calc = new Functions; //Used for random number generation
    Test* print = new Test; //Used to output debug lines
    MultipleObjects* objects = new MultipleObjects; //Facilitates multiple LSCs
    
    MATLABPrint* matlab = new MATLABPrint;
    
    vector<vector<Point3D>> paths;
    
    //World dimensions and settings
    Point3D A (0,0,0);
    Point3D B (20,0,0);
    Point3D C (0,20,0);
    double h = 20;
    
    Sheet* worldbase = new Sheet;
    worldbase->Set(A,B,C);
    
    world->SetRefractiveIndex(1);
    world->SetConcentration(0);
    world->Set(worldbase, h);
    
    
    //LSC dimensions and settings
    Sheet* lscbase = new Sheet;
    Material* lsc = new Material;
    
    Sheet* lscbase2 = new Sheet;
    Material* lsc2 = new Material;
    
    lsc->ReadData(1);
    lsc2->ReadData(1);
    
    Point3D D (1,1,1);
    Point3D E (11,1,1);
    Point3D F (1,11,1);
    double h2 = 0.5;
    
    Point3D J (1,1,2);
    Point3D K (11,1,2);
    Point3D L (1,11,2);
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
    
    objects->StoreWorld(world); //Stores world
    
    //Stores LSC
    if(lscs>=1)objects->StoreMaterial(lsc);
    if(lscs==2) objects->StoreMaterial(lsc2);
    
    if(matlabprint) matlab->PrintLSCs(objects);
    
    double hits = 0;
    double photons = 0;
    
    vector<double> output;
    vector<Point3D> dyeabs;
    
    //Loop for each wavelength. Set wavelength Range here.
    
    for(int wavelength = start; wavelength<= end; wavelength++){
        
        double thisphotons = 0;
        double thishits = 0;
        
        //Loop for individual wavelength.
        
        for(int i = 0; i<runs; i++){
            
            vector<Point3D> photonpath;
            
            //New photon settings
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
                cout<<"New photon:"<<endl<<endl;
                print->PhotonPrint(photon);
            }
            
            while(world->ReturnPhotonInside() & photon->PhotonAliveCheck()){ //While photon is inside world and alive.
                if(!objects->PhotonInMaterial()){ //If photon is not in a LSC.
                    if(world->GetInterfaceDistance(photon)<objects->NextInterfaceDistance(photon)){ //If next boundary is exit.
                        photon->PhotonKill();
                        world->SetPhotonInside(0);
                        if(debug) {
                            cout<<"World exit."<<endl<<endl;
                        }
                    }
                    
                    else{
                        inout->In(photon, world, objects->NextInterfaceMaterial(photon), debug); //Else, entrance reflect/refract event.
                        if(matlabprint && objects->PhotonInMaterial()) photonpath.push_back(photon->GetPosition());
                    }
                }
                
                
                else while(objects->PhotonInMaterial()&&photon->PhotonAliveCheck()){ //while photon is in LSC.
                    
                    if(photon->GetAbsorbLength()<=objects->CurrentMaterial()->GetInterfaceDistance(photon)){ //If absorption = next event.
                        objects->CurrentMaterial()->AbsorptionEvent(photon,debug,matlabprint,dyeabs,photonpath); //Absorption event.
                    }
                    
                    else{ //If boundary is next event.
                        
                        if(!(objects->CurrentMaterial()->GetInterfaceSheet(photon) == objects->CurrentMaterial()->GetBase()) & !(objects->CurrentMaterial()->GetInterfaceSheet(photon) == objects->CurrentMaterial()->GetTop())){
                            photon->PhotonKill(); //If sheet is not top or bottom. Kill photon + add counters.
                            hits++;
                            thishits++;
                            if(debug){
                                cout<<"Hit at interface point: ";
                                print->PrintPoint(objects->CurrentMaterial()->GetInterfacePoint(photon));
                                cout<<endl;
                            }
                        }
                        else{
                            inout->Out(photon, objects->NextInterfaceMaterial(photon), objects->CurrentMaterial(), debug, objects); //Otherwise exit reflect/refract event.
                            if(matlabprint && world->PointinBox(photon)) photonpath.push_back(photon->GetPosition());

                        }
                    }
                }
            }
            
            //Counters
            
            //Deletes photon.
            
            world->SetPhotonInside(0);
            objects->ResetPhotonsInside();
            paths.push_back(photonpath);
            delete photon;
            
        }
        
        //Adds value for individual wavelengths to vector.
        
        output.push_back(100*thishits/thisphotons);
        
    }
    
    //Prints vectors to files for individual wavelengths.
    
    print->PrintVectorFile(output, "output.txt");
    
    //Calculates total efficiency and prints as 'result'
    
    double result = 100 * hits/photons;
    vector<double> finalresult;
    finalresult.push_back(result);
    print->PrintVectorFile(finalresult,"efficiency.txt");
    
    if(matlabprint) matlab->DyeAbsorbPrint(dyeabs);
    cout<<"Total Optical Efficiency: "<<result<<"%"<<endl;
    
    if(matlabprint) matlab->PhotonPathPrint(paths);
    
}

int main(int argc, const char * argv[]){    
    run(10,2,450,450,0,1);
}
