#include <iostream>
#include "Test.h"
#include "Functions.h"
#include "FresnelJackson.hpp"
#include <vector>
#include "MultipleObjects.h"
#include "MATLABPrint.h"
#include "Spherical3D.h"
#include "anisotropic.h"
#include "unittests.hpp"
#include "curvedbox.hpp"
#include "flexi.hpp"
#include "intersection.hpp"
#include "arch.hpp"
#include "curvedlsc.hpp"
#include "probdistribution.hpp"
#include "scattering.hpp"
#include "matrixread.hpp"
#include "spectra.hpp"


using namespace std;

void run(double runs,int lscs, int start, int end, bool debug, bool matlabprint, bool wavelengthprint){
    
    bool scatter = 0;
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
    
    double r = 0;
    bool hot = 0;
    
    lsc->ReadData(1,0,r,hot);
    lsc2->ReadData(1,0,r,hot);
    
    
    Point3D D (1,1,1);
    Point3D E (11,1,1);
    Point3D F (1,11,1);
    double h2 = 0.4;
    
    Point3D J (1,1,2);
    Point3D K (11,1,2);
    Point3D L (1,11,2);
    double h3 = 0.5;
    
    double squareradius = 9.192388155/10;
    
    
    Point3D G (6-squareradius,6-squareradius,9);
    Point3D H (6+squareradius,6-squareradius,9);
    Point3D I (6-squareradius,6+squareradius,9);
    
    
    
    Sheet* source = new Sheet;
    
    source->Set(G,H,I);
    
    lscbase->Set(D,E,F);
    lsc->Set(lscbase,h2);
    
    lscbase2->Set(J,K,L);
    lsc2->Set(lscbase2,h3);
    
    lsc->SetRefractiveIndex(1.495);
    lsc->SetConcentration(1e-4);
    
    lsc2->SetRefractiveIndex(1.495);
    lsc2->SetConcentration(1e-5);
    
    objects->StoreWorld(world); //Stores world
    
    //Stores LSC
    if(lscs>=1)objects->StoreMaterial(lsc);
    if(lscs==2) objects->StoreMaterial(lsc2);
    
    if(matlabprint) matlab->PrintLSCs(objects);
    
    double hits = 0;
    double photons = 0;
    
    //Lists storing output files
    
    vector<double> output;
    vector<Point3D> dyeabs;
    
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
            
            vector<Point3D> photonpath;
            
            
            //New photon settings
            Photon* photon = new Photon;
            
            photons++;
            thisphotons++;
            
            double sx, sy;
            
            sx = source->GetA().x + calc->Random(1) * source->GetABLength();
            sy = source->GetB().y + calc->Random(1) * source->GetACLength();
            
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
                        inout->In(photon, world, objects->NextInterfaceMaterial(photon), debug, scatter); //Else, entrance reflect/refract event.
                        if(matlabprint && objects->PhotonInMaterial()) photonpath.push_back(photon->GetPosition());
                    }
                }
                
                
                else while(objects->PhotonInMaterial()&&photon->PhotonAliveCheck()){ //while photon is in LSC.
                    
                    if(photon->GetAbsorbLength()<=objects->CurrentMaterial()->GetInterfaceDistance(photon)){ //If absorption = next event.
                        bool d = 0;
                        double b = 0;
                        objects->CurrentMaterial()->AbsorptionEvent(photon,debug,matlabprint,dyeabs,photonpath,d,b); //Absorption event.
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
                            inout->Out(photon, objects->NextInterfaceMaterial(photon), objects->CurrentMaterial(), debug, objects, 0); //Otherwise exit reflect/refract event.
                            if(matlabprint && world->PointinBox(photon)) photonpath.push_back(photon->GetPosition());
                            
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
            paths.push_back(photonpath);
            delete photon;
            
        }
        
        //Adds value for individual wavelengths to vector.
        
        output.push_back(100*thishits/thisphotons);
        
        inside.push_back(100*thisinside/thisphotons);
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
        
        if(wavelengthprint){
            cout<<"Wavelength "<<wavelength<<" done."<<endl;
        }
        
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
    
    if(matlabprint) matlab->DyeAbsorbPrint(dyeabs);
    cout<<"Total Optical Efficiency: "<<result<<"%"<<endl;
    
    if(matlabprint) matlab->PhotonPathPrint(paths);
    
}

void flexirun(double runs, int start, int end, bool matlabprint, bool debug, bool fulldebug, bool wavelengthprint){
    
    //Main algorithm. runs = runs per wavelength. debug = debug mode.
    
    //Creates environment
    
    Material* world = new Material; //Creates new world box.
    
    FresnelJackson* inout = new FresnelJackson; //Calculation for boundarys
    Functions* calc = new Functions; //Used for random number generation
    Test* print = new Test; //Used to output debug lines
    
    MATLABPrint* matlab = new MATLABPrint;
    
    vector<vector<Point3D>> paths;
    
    //World dimensions and settings
    Point3D A (-50,-50,-50);
    Point3D B (50,-50,-50);
    Point3D C (-50,50,-50);
    double h = 100;
    
    Sheet* worldbase = new Sheet;
    worldbase->Set(A,B,C);
    
    world->SetRefractiveIndex(1);
    world->SetConcentration(0);
    world->Set(worldbase, h);
    
    //LSC dimensions and parameters
    
    
    
    //Setting LSC parameters with a given LSC length, l, radius of curvature r. This gives output angles for arc. values a and b must be equal.
    
    double r = 30; //radius of curvature
    double l = 10; //length of lsc
    double height = 10; //height of lsc
    double width = 0.4; //width of lsc/thickness
    Point3D centrepoint(-(r),10,0);
    ellipse e(centrepoint,r,r);
    e.SetA(r);
    e.SetB(r);
    arc arcy(e,-(l/(2*r)),(l/(2*r)));
    
    
    curvedbox* LSC = new curvedbox(arcy,height,width,1.495);
    double rzz=0;
    bool hot = 0;
    LSC->ReadData(1,0,rzz,hot);
    LSC->SetConcentration(1e-4);
    LSC->Set(arcy, height, width, 1.495);
    
    double squareradius = 9.192388155/10;
    
    
    Point3D SourceA (30,10-squareradius,5-squareradius);
    Point3D SourceB (30,10+squareradius,5-squareradius);
    Point3D SourceC (30,10-squareradius,5+squareradius);
    
    Sheet* source = new Sheet;
    
    source->Set(SourceA,SourceB,SourceC);
    
    
    double hits = 0;
    double absorbed = 0;
    double photons = 0;
    
    //Lists storing output files
    
    vector<double> output;
    vector<Point3D> dyeabs;
    
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
    
    vector<double> InternalEfficiency;
    
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
            
            vector<Point3D> photonpath;
            
            
            //New photon settings
            Photon* photon = new Photon;
            
            photons++;
            thisphotons++;
            
            double sy, sz;
            
            sy = source->GetA().y + calc->Random(1) * source->GetABLength();
            sz = source->GetA().z + calc->Random(1) * source->GetACLength();
            
            photon->SetPosition(Point3D(50,sy,sz));
            photon->SetMomentum(Vector3D(-1,0,0));
            photon->SetWavelength(wavelength);
            photon->SetRandomPolarisation();
            
            world->CorrectPhotonInside(photon);
            
            if(debug) {
                cout<<"New photon:"<<endl<<endl;
                print->PhotonPrint(photon);
            }
            
            while(world->ReturnPhotonInside() & photon->PhotonAliveCheck()){ //While photon is inside world and alive.
                if(!LSC->GetPhotonInside()){ //If photon is not in a LSC.
                    if(fulldebug){
                        cout<<"World Exit Distance is "<<world->GetInterfaceDistance(photon)<<"."<<endl;
                    }
                    
                    if(world->GetInterfaceDistance(photon)<LSC->NextInterfaceDistance(*photon,fulldebug)){ //If next boundary is exit.
                        photon->PhotonKill();
                        photon->SetExit();
                        world->SetPhotonInside(0);
                        if(debug) {
                            cout<<"World exit."<<endl<<endl;
                        }
                    }
                    
                    else{
                        inout->CurvedIn(photon, world, LSC, debug); //Else, entrance reflect/refract event.
                        if(matlabprint && LSC->GetPhotonInside()) photonpath.push_back(photon->GetPosition());
                    }
                }
                
                
                else while(LSC->GetPhotonInside()&&photon->PhotonAliveCheck()){ //while photon is in LSC.
                    
                    if(photon->GetAbsorbLength()<=LSC->NextInterfaceDistance(*photon,fulldebug)){ //If absorption = next event.
                        double d=0;
                        bool b=0;
                        LSC->AbsorptionEvent(photon,debug,matlabprint,dyeabs,photonpath,b,d); //Absorption event.
                    }
                    
                    else{ //If boundary is next event.
                        
                        int nextinterface = LSC->NextInterface(*photon,fulldebug);
                        
                        if((nextinterface!=2 && nextinterface!=3)){
                            photon->PhotonKill(); //If sheet is not inside or outside. Kill photon + add counters.
                            hits++;
                            thishits++;
                            if(debug){
                                cout<<"Hit on sheet "<<nextinterface<<" at interface point: ";
                                print->PrintPoint(photon->GetPosition()+photon->GetMomentum()*LSC->NextInterfaceDistance(*photon,fulldebug));
                                cout<<endl;
                            }
                        }
                        else{
                            inout->CurvedOut(photon, LSC, world, debug); //Otherwise exit reflect/refract event.
                            if(matlabprint && world->PointinBox(photon)) photonpath.push_back(photon->GetPosition());
                            
                        }
                    }
                }
            }
            
            //Counters
            
            if(photon->GetAbsorptions()!=0) absorbed++;
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
            if(photon->GetAbsorptions()==0 && photon->GetInside() && photon->GetExit()) notabsorbedinside++;
            if(photon->GetInside()&&photon->GetAbsorptions()!=0&&photon->GetExit()) insideabsorbedexit++;
            
            //Deletes photon.
            
            world->SetPhotonInside(0);
            LSC->SetPhotonInside(0);
            paths.push_back(photonpath);
            delete photon;
            
        }
        
        //Adds value for individual wavelengths to vector.
        
        output.push_back(100*thishits/thisphotons);
        
        inside.push_back(100*thisinside/thisphotons);
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
        
        if(wavelengthprint){
            cout<<"Wavelength "<<wavelength<<"nm done."<<endl;
        }
        
        
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
    double internal_eff = 100 * hits/absorbed;
    vector<double> finalresult;
    finalresult.push_back(result);
    finalresult.push_back(internal_eff);
    print->PrintVectorFile(finalresult,"efficiency.txt");
    
    if(matlabprint) matlab->DyeAbsorbPrint(dyeabs);
    cout<<"Total Optical Efficiency: "<<result<<"%"<<endl;
    cout<<"Internal Optical Efficiency: "<<internal_eff<<"%"<<endl;
    
    if(matlabprint) matlab->PhotonPathPrint(paths);
}

void flexirun_pof_new(double runs, int start, int end, bool matlabprint, bool debug, bool fulldebug, bool wavelengthprint, double rad){
    
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
    Point3D A (-10000,-10000,-10000);
    Point3D B (10000,-10000,-10000);
    Point3D C (-10000,10000,-10000);
    double h = 20000;
    
    Sheet* worldbase = new Sheet;
    worldbase->Set(A,B,C);
    
    world->SetRefractiveIndex(1.402);
    world->SetConcentration(0);
    world->Set(worldbase, h);
    
    double rm = 0;
    bool hot = 0;
    world->ReadData(1,0, rm, hot);
    
    //LSC dimensions and parameters
    
    
    
    //Setting LSC parameters with a given LSC length, l, radius of curvature r. This gives output angles for arc. values a and b must be equal.
    
    double r = rad; //radius of curvature
    double l = M_PI*r; //length of lsc
    double height = 0.98; //height of lsc
    double width = 0.98; //width of lsc/thickness
    
    
    Point3D centrepoint(-(r),0,0);
    
    Point2D centre2D(centrepoint.x,centrepoint.y);
    
    circle cen;
    cen.SetCentre(centre2D);
    cen.SetRadius(r);
    
    tube tub;
    tub.SetCircle(cen);
    tub.SetHeight(height);
    
    arch arc;
    arc.SetTube(tub);
    double st = -M_PI_2;
    double en = - st;
    arc.SetStart(st);
    arc.SetEnd(en);
    
    curvedlsc LSC;
    
    ellipse e(centrepoint,r,r);
    e.SetA(r);
    e.SetB(r);
    
    double n = 1.492;
    
    LSC.Set(centrepoint, r, l, width, height, n);
    
    double rc = 0;
    
    LSC.ReadData(1,0,rc,hot);
    LSC.SetConcentration(0);
    
    double squareradius = 9.192388155/10;
    
    
    Point3D SourceA (30,10-squareradius,5-squareradius);
    Point3D SourceB (30,10+squareradius,5-squareradius);
    Point3D SourceC (30,10-squareradius,5+squareradius);
    
    Point3D SSA, SSB, SSC;
    
    SSA = LSC.GetSheet(2, debug).GetA();
    SSB = LSC.GetSheet(2, debug).GetB();
    SSC = LSC.GetSheet(2, debug).GetC();
    
    Sheet* source = new Sheet;
    
    source->Set(SSA,SSB,SSC);
    Vector3D sdir = source->GetNormal();
    
    objects->StoreWorld(world); //Stores world
    objects->StoreMaterial(&LSC);
    
    
    double hits = 0;
    double absorbed = 0;
    double photons = 0;
    double total_reflects = 0;
    
    //Lists storing output files
    
    vector<double> output;
    vector<Point3D> dyeabs;
    
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
    
    vector<double> InternalEfficiency;
    
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
        
        double reflection = 0;
        
        
        //Loop for individual wavelength.
        
        for(int i = 0; i<runs; i++){
            
            vector<Point3D> photonpath;
            
            
            //New photon settings
            Photon* photon = new Photon;
            
            photons++;
            thisphotons++;
            
            
            double AB, AC;
            
            AB = source->GetA().distancetopoint(source->GetB());
            AC = source->GetA().distancetopoint(source->GetC());
            
            Vector3D VEC_AB, VEC_AC;
            VEC_AB = source->GetB() - source->GetA();
            VEC_AC = source->GetC() - source->GetA();
            
            //Point3D spos = source->GetA() + calc->Random(1) * VEC_AC + calc->Random(1) * VEC_AB + sdir;
            
            double dist = 1.859375;
            
            Point3D cent_spos = source->Centre() + dist* sdir;
            
            double ydir = (width/2)*(acos(1-(2*calc->Random(1)))-M_PI_2)/M_PI_2;
            double xdir = dist;
            double zdir = 0;
            
            Vector3D newdir(xdir,ydir,zdir);
            newdir.Normalise();
            
            photon->SetPosition(cent_spos);
            photon->SetMomentum(newdir);
            photon->SetWavelength(wavelength);
            photon->SetRandomPolarisation();
            
            world->SetPhotonInside(1);
            
            if(debug) {
                cout<<"New photon:"<<endl<<endl;
                print->PhotonPrint(photon);
            }
            
            while(world->ReturnPhotonInside() & photon->PhotonAliveCheck()){ //While photon is inside world and alive.
                if(!LSC.ReturnPhotonInside()){ //If photon is not in a LSC.
                    
                    LSC.FindIntersections(*photon, debug);
                    
                    if(fulldebug){
                        cout<<"World Exit Distance is "<<world->GetInterfaceDistance(photon)<<"."<<endl<<endl;
                    }
                    
                    if(world->GetInterfaceDistance(photon)<LSC.NextDistance(*photon, debug)){ //If next boundary is exit.
                        photon->PhotonKill();
                        photon->SetExit();
                        world->SetPhotonInside(0);
                        if(debug) {
                            cout<<"World exit."<<endl<<endl;
                        }
                    }
                    
                    else{
                        inout->NewCurvedIn(photon, world, LSC, debug); //Else, entrance reflect/refract event.
                    }
                }
                
                
                else while(LSC.ReturnPhotonInside()&&photon->PhotonAliveCheck()){ //while photon is in LSC.
                    
                    LSC.FindIntersections(*photon, debug);
                    
                    int nextinterface = LSC.NextIntersection(*photon, debug);
                    
                    if(nextinterface==3){
                        photon->PhotonKill(); //If sheet is not inside or outside. Kill photon + add counters.
                        hits++;
                        thishits++;
                        if(debug){
                            cout<<"Hit on sheet "<<nextinterface<<" at interface point: ";
                            print->PrintPoint(photon->GetPosition()+photon->GetMomentum()*LSC.NextDistance(*photon, debug));
                            cout<<endl;
                        }
                    }
                    else{
                        inout->NewCurvedOut(photon, LSC, world, debug,reflection); //Otherwise exit reflect/refract event.
                        
                        
                    }
                }
            }
            
            //Counters
            
            if(photon->GetAbsorptions()!=0) absorbed++;
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
            if(photon->GetAbsorptions()==0 && photon->GetInside() && photon->GetExit()) notabsorbedinside++;
            if(photon->GetInside()&&photon->GetAbsorptions()!=0&&photon->GetExit()) insideabsorbedexit++;
            
            //Deletes photon.
            
            world->SetPhotonInside(0);
            LSC.SetPhotonInside(0);
            delete photon;
            
        }
        
        //Adds value for individual wavelengths to vector.
        
        output.push_back(100*thishits/thisphotons);
        total_reflects = total_reflects + reflection;
        
        inside.push_back(100*thisinside/thisphotons);
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
        
        if(wavelengthprint){
            cout<<"Wavelength "<<wavelength<<"nm done."<<endl;
        }
        
        
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
    double internal_eff = 100 * hits/absorbed;
    vector<double> finalresult;
    finalresult.push_back(result);
    finalresult.push_back(internal_eff);
    print->PrintVectorFile(finalresult,"efficiency.txt");
    
    if(matlabprint) matlab->DyeAbsorbPrint(dyeabs);
    cout<<"Total Optical Efficiency: "<<result<<"%"<<endl;
    cout<<"Internal Optical Efficiency: "<<internal_eff<<"%"<<endl;
    cout<<"Average reflections = "<<total_reflects/photons<<endl;
    
    
    if(matlabprint) matlab->PhotonPathPrint(paths);
}

void flexirun_new(double runs, int start, int end, bool matlabprint, bool debug, bool fulldebug, bool wavelengthprint, double rads){
    
    //Main algorithm. runs = runs per wavelength. debug = debug mode.
    
    //Creates environment
    
    Material* world = new Material; //Creates new world box.
    
    FresnelJackson* inout = new FresnelJackson; //Calculation for boundarys
    Functions* calc = new Functions; //Used for random number generation
    Test* print = new Test; //Used to output debug lines
    
    MATLABPrint* matlab = new MATLABPrint;
    
    vector<vector<Point3D>> paths;
    
    //World dimensions and settings
    Point3D A (-500,-500,-500);
    Point3D B (500,-500,-500);
    Point3D C (-500,500,-500);
    double h = 1000;
    
    Sheet* worldbase = new Sheet;
    worldbase->Set(A,B,C);
    
    world->SetRefractiveIndex(1);
    world->SetConcentration(0);
    world->Set(worldbase, h);
    
    //LSC dimensions and parameters
    
    
    
    //Setting LSC parameters with a given LSC length, l, radius of curvature r. This gives output angles for arc. values a and b must be equal.
    
    
    double r = rads; //radius of curvature
    double l = 100; //length of lsc
    double height = 100; //height of lsc
    double width = 0.4; //width of lsc/thickness
    
    
    Point3D centrepoint(-(r),0,0);
    
    Point2D centre2D(centrepoint.x,centrepoint.y);
    
    circle cen;
    cen.SetCentre(centre2D);
    cen.SetRadius(r);
    
    tube tub;
    tub.SetCircle(cen);
    tub.SetHeight(height);
    
    arch arc;
    arc.SetTube(tub);
    double st = -l/(2*r);
    double en = - st;
    arc.SetStart(st);
    arc.SetEnd(en);
    
    double n = 1.495;
    
    
    curvedlsc* LSC = new curvedlsc;
    
    double rz = 0;
    bool hot = 0;
    
    LSC->ReadData(1,0,rz,hot);
    LSC->Set(centrepoint, r, l, width, height, n);
    
    //double squareradius = 9.192388155/10;
    double squareradius = 50;
    
    //Point3D SourceA(30,0,5);
    //Point3D SourceB(30,0,5);
    //Point3D SourceC(30,0,5);
    
    Point3D SourceA (30,0-squareradius,5-squareradius);
    Point3D SourceB (30,0+squareradius,5-squareradius);
    Point3D SourceC (30,0-squareradius,5+squareradius);
    
    Sheet* source = new Sheet;
    
    source->Set(SourceA,SourceB,SourceC);
    
    
    double hits = 0;
    double absorbed = 0;
    double photons = 0;
    
    
    //Lists storing output files
    
    vector<double> output;
    vector<Point3D> dyeabs;
    
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
    
    vector<double> InternalEfficiency;
    double reflections;
    
    //Loop for each wavelength. Set wavelength Range here.
    
        LSC->SetConcentration(1e-4);
        
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
                
                vector<Point3D> photonpath;
                
                
                //New photon settings
                Photon* photon = new Photon;
                
                photons++;
                thisphotons++;
                
                double sy, sz;
                
                sy = source->GetA().y + calc->Random(1) * source->GetABLength();
                sz = source->GetA().z + calc->Random(1) * source->GetACLength();
                
                
                
                photon->SetPosition(Point3D(50,sy,sz));
                photon->SetMomentum(Vector3D(-1,0,0));
                photon->SetWavelength(wavelength);
                photon->SetRandomPolarisation();
                
                world->CorrectPhotonInside(photon);
                
                if(debug) {
                    cout<<"New photon:"<<endl<<endl;
                    print->PhotonPrint(photon);
                }
                
                while(world->ReturnPhotonInside() & photon->PhotonAliveCheck()){ //While photon is inside world and alive.
                    if(!LSC->ReturnPhotonInside()){ //If photon is not in a LSC.
                        
                        LSC->FindIntersections(*photon, debug);
                        
                        if(fulldebug){
                            cout<<"World Exit Distance is "<<world->GetInterfaceDistance(photon)<<"."<<endl;
                        }
                        
                        if(world->GetInterfaceDistance(photon)<LSC->NextDistance(*photon,fulldebug)){ //If next boundary is exit.
                            photon->PhotonKill();
                            photon->SetExit();
                            world->SetPhotonInside(0);
                            if(debug) {
                                cout<<"World exit."<<endl<<endl;
                            }
                        }
                        
                        else{
                            inout->NewCurvedIn(photon, world, *LSC, debug); //Else, entrance reflect/refract event.
                            if(matlabprint && LSC->ReturnPhotonInside()) photonpath.push_back(photon->GetPosition());
                        }
                    }
                    
                    
                    else while(LSC->ReturnPhotonInside()&&photon->PhotonAliveCheck()){ //while photon is in LSC.
                        
                        LSC->FindIntersections(*photon, debug);
                        
                        
                        if(photon->GetAbsorbLength()<=LSC->NextDistance(*photon,fulldebug)){ //If absorption = next event.
                            bool d = 0;
                            double b = 0;
                            LSC->AbsorptionEvent(photon,debug,matlabprint,dyeabs,photonpath,d,b); //Absorption event.
                        }
                        
                        else{ //If boundary is next event.
                            
                            int nextinterface = LSC->NextIntersection(*photon,fulldebug);
                            
                            if((nextinterface!=0 && nextinterface!=1)){
                                photon->PhotonKill(); //If sheet is not inside or outside. Kill photon + add counters.
                                hits++;
                                thishits++;
                                if(debug){
                                    cout<<"Hit on sheet "<<nextinterface<<" at interface point: ";
                                    print->PrintPoint(photon->GetPosition()+photon->GetMomentum()*LSC->NextDistance(*photon,fulldebug));
                                    cout<<endl;
                                }
                            }
                            else{
                                inout->NewCurvedOut(photon, *LSC, world, debug, reflections); //Otherwise exit reflect/refract event.
                                if(matlabprint && world->PointinBox(photon)) photonpath.push_back(photon->GetPosition());
                                
                            }
                        }
                    }
                }
                
                //Counters
                
                if(photon->GetAbsorptions()!=0) absorbed++;
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
                if(photon->GetAbsorptions()==0 && photon->GetInside() && photon->GetExit()) notabsorbedinside++;
                if(photon->GetInside()&&photon->GetAbsorptions()!=0&&photon->GetExit()) insideabsorbedexit++;
                
                //Deletes photon.
                
                world->SetPhotonInside(0);
                LSC->SetPhotonInside(0);
                paths.push_back(photonpath);
                delete photon;
                
            }
            
            //Adds value for individual wavelengths to vector.
            
            output.push_back(100*thishits/thisphotons);
            inside.push_back(100*thisinside/thisphotons);
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
            
            if(wavelengthprint){
                cout<<"Wavelength "<<wavelength<<"nm done."<<endl;
            }
            
            
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
        double internal_eff = 100 * hits/absorbed;
        vector<double> finalresult;
        finalresult.push_back(result);
        finalresult.push_back(internal_eff);
        print->PrintVectorFile(finalresult,"efficiency.txt");
        
        if(matlabprint) matlab->DyeAbsorbPrint(dyeabs);
        cout<<"Total Optical Efficiency: "<<result<<"%"<<endl;
        cout<<"Internal Optical Efficiency: "<<internal_eff<<"%"<<endl;
        
        if(matlabprint) matlab->PhotonPathPrint(paths);
}

int hybrid(double runs, int lscs, int start, int end, bool debug, bool wavelengthprint, bool hot, double radius, double conc1, double thickness){
    
    /*               KEY:
     runs    -   Photons/Wavelength.
     lscs    -   Allows for creation of multiple lscs.
     start   -   start wavelength.
     end     -   end wavelength.
     debug   -   set this to 1 for debug mode.
     bool    -   wavelengthprint.       */
    
    
    Material* world = new Material; //Creates new world box.
    FresnelJackson* inout = new FresnelJackson; //Calculation for boundarys
    Functions* calc = new Functions; //Used for random number generation
    Test* print = new Test; //Used to output debug lines
    MultipleObjects* objects = new MultipleObjects; //Facilitates multiple LSCs
    scattering scatterdata; // Used for scaterring calculations
    bool matlabprint = 0;
    
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
    
    lsc->ReadData(1,1,radius,hot);
    lsc2->ReadData(1,1,radius,hot);
    
    
    Point3D D (1,1,1);
    Point3D E (11,1,1);
    Point3D F (1,11,1);
    double h2 = thickness;
    
    Point3D J (1,1,2);
    Point3D K (11,1,2);
    Point3D L (1,11,2);
    double h3 = 0.5;
    
    double squareradius = 9.192388155/10;
    
    
    Point3D G (6-squareradius,6-squareradius,9);
    Point3D H (6+squareradius,6-squareradius,9);
    Point3D I (6-squareradius,6+squareradius,9);
    
    
    Sheet* source = new Sheet;
    
    source->Set(G,H,I);
    
    lscbase->Set(D,E,F);
    lsc->Set(lscbase,h2);
    
    lscbase2->Set(J,K,L);
    lsc2->Set(lscbase2,h3);
    
    lsc->SetRefractiveIndex(1.495);
    lsc->SetConcentration(conc1);
    
    lsc2->SetRefractiveIndex(1.495);
    lsc2->SetConcentration(1e-5);
    
    objects->StoreWorld(world); //Stores world
    
    //Stores LSC
    if(lscs>=1)objects->StoreMaterial(lsc);
    if(lscs==2) objects->StoreMaterial(lsc2);
    
    double photons = 0;
    double transmission = 0;
    double totalhazecone = 0;
    
    
    //Lists storing output files
    
    vector<double> hazecone;
    vector<double> transmission_lambda;
    vector<Point3D> dyeabs;
    
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
    
    double hazeangle_deg = 5;
    double hazeangle_rad = hazeangle_deg * M_PI / 180;
    double coshaze = cos(hazeangle_rad);
    
    //Loop for each wavelength. Set wavelength Range here.
    
    for(int wavelength = start; wavelength<= end; wavelength=wavelength+10){
        
        scatterdata.GetDistributions(radius, wavelength, hot);
        
        double thisphotons = 0;
        double thistransmission = 0;
        double thishazecone = 0;
        
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
        
        bool scatter = 1;
        
        //Loop for individual wavelength.
        
        for(int i = 0; i<runs; i++){
            
            vector<Point3D> photonpath;
            
            
            //New photon settings
            Photon* photon = new Photon;
            
            photons++;
            thisphotons++;
            
            double sx, sy;
            
            sx = source->GetA().x + calc->Random(1) * source->GetABLength();
            sy = source->GetB().y + calc->Random(1) * source->GetACLength();
            
            Vector3D entrance(0,0,-1);
            
            photon->SetPosition(Point3D(sx,sy,9));
            photon->SetMomentum(entrance);
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
                        inout->In(photon, world, objects->NextInterfaceMaterial(photon), debug, scatter); //Else, entrance reflect/refract event.
                    }
                }
                
                
                else while(objects->PhotonInMaterial()&&photon->PhotonAliveCheck()){ //while photon is in LSC.
                    
                    double Scat = photon->GetScatterLength();
                    double Absorb = photon->GetAbsorbLength();
                    double Interface = objects->CurrentMaterial()->GetInterfaceDistance(photon);
                    
                    if(Absorb<=Interface || Scat<=Interface){ //If absorption/scatter = next event.
                        if(Absorb<=Scat){ //Absorption event is next.
                            objects->CurrentMaterial()->AbsorptionEvent(photon,debug,matlabprint,dyeabs,photonpath,hot,radius);
                        }else{
                            objects->CurrentMaterial()->ScatterEvent(photon, debug, matlabprint, dyeabs, photonpath, scatterdata, hot, radius);
                        }
                    }
                    
                    else{ //If boundary is next event.
                        
                        if((objects->CurrentMaterial()->GetInterfaceSheet(photon) == objects->CurrentMaterial()->GetBase())){
                            if(inout->Out(photon, objects->NextInterfaceMaterial(photon), objects->CurrentMaterial(), debug, objects,1)){
                                transmission++;
                                thistransmission++;
                                if(Dot(entrance,photon->GetMomentum())>=coshaze){
                                    thishazecone++;
                                    totalhazecone++;
                                }
                            }//If transmittion, add
                            
                        }else{
                            inout->Out(photon, objects->NextInterfaceMaterial(photon), objects->CurrentMaterial(), debug, objects,1); //O therwise exit reflect/refract event.
                            if(matlabprint && world->PointinBox(photon)) photonpath.push_back(photon->GetPosition());
                            
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
        
        transmission_lambda.push_back(100*thistransmission/thisphotons);
        hazecone.push_back(100*thishazecone/thistransmission);
        
        inside.push_back(100*thisinside/thisphotons);
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
        
        if(wavelengthprint){
            cout<<"Wavelength "<<wavelength<<" done. Transmission:"<<100*thistransmission/thisphotons<<"%. Inside hazecone:"<<100*thishazecone/thistransmission<<"%"<<endl;
        }
        
    }
    
    double conc_thickness = conc1*thickness;
    
    //Prints vectors to files for individual wavelengths.
    
    ostringstream t_lambda;
    ostringstream h_lambda;
    
    if(hot){
        t_lambda<<"r"<<radius<<"_hot_transmission"<<conc_thickness<<".txt";
        h_lambda<<"r"<<radius<<"_hot_haze"<<conc_thickness<<".txt";
        
    }else{
        t_lambda<<"r"<<radius<<"_cold_transmission"<<conc_thickness<<".txt";
        h_lambda<<"r"<<radius<<"_cold_haze"<<conc_thickness<<".txt";
    }
    
    
    
    print->PrintVectorFile(transmission_lambda, t_lambda.str());
    print->PrintVectorFile(hazecone, h_lambda.str());
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
    
    double result = 100 * transmission/photons;
    vector<double> finalresult;
    finalresult.push_back(result);
    print->PrintVectorFile(finalresult,"efficiency.txt");
    
    cout<<"Transmission: "<<result<<"%"<<endl;
    cout<<"of which inside haze angle of "<<hazeangle_deg<<" degrees: "<<100*totalhazecone/transmission<<"%"<<endl;
    
    return 0;
}

void testsesh(){
    spectra am;
    am.Import("spectrum.txt");
    cout<<am.GetMatrix().GetValue(1, 0);
    
}

//Eliminating all extra stored data, creating just sweep for efficiencies
void parametersweep(double runs, int start, int end, bool matlabprint, bool debug, bool fulldebug, bool wavelengthprint, double rads){
    
    //Main algorithm. runs = runs per wavelength. debug = debug mode.
    
    //Creates environment
    
    Material* world = new Material; //Creates new world box.
    
    FresnelJackson* inout = new FresnelJackson; //Calculation for boundarys
    Functions* calc = new Functions; //Used for random number generation
    Test* print = new Test; //Used to output debug lines
    
    MATLABPrint* matlab = new MATLABPrint;
    
    vector<vector<Point3D>> paths;
    
    //World dimensions and settings
    Point3D A (-500,-500,-500);
    Point3D B (500,-500,-500);
    Point3D C (-500,500,-500);
    double h = 1000;
    
    Sheet* worldbase = new Sheet;
    worldbase->Set(A,B,C);
    
    world->SetRefractiveIndex(1);
    world->SetConcentration(0);
    world->Set(worldbase, h);
    
    //LSC dimensions and parameters
    
    //Setting LSC parameters with a given LSC length, l, radius of curvature r. This gives output angles for arc. values a and b must be equal.
    
    vector<vector<double>> externalsweep;
    vector<vector<double>> internalsweep;
    
    
    
    for(int thickness_run = 1; thickness_run<3; thickness_run++){

    
    double r = rads; //radius of curvature
    double l = 100; //length of lsc
    double height = 100; //height of lsc
    double width = 0.4*thickness_run; //width of lsc/thickness
    
    
    Point3D centrepoint(-(r),0,0);
    
    Point2D centre2D(centrepoint.x,centrepoint.y);
    
    circle cen;
    cen.SetCentre(centre2D);
    cen.SetRadius(r);
    
    tube tub;
    tub.SetCircle(cen);
    tub.SetHeight(height);
    
    arch arc;
    arc.SetTube(tub);
    double st = -l/(2*r);
    double en = - st;
    arc.SetStart(st);
    arc.SetEnd(en);
    
    double n = 1.495;
    
    
    curvedlsc* LSC = new curvedlsc;
    
    double rz = 0;
    bool hot = 0;
    
    LSC->ReadData(1,0,rz,hot);
    LSC->Set(centrepoint, r, l, width, height, n);
    
    double squareradius = 50;
    
    Point3D SourceA (30,0-squareradius,5-squareradius);
    Point3D SourceB (30,0+squareradius,5-squareradius);
    Point3D SourceC (30,0-squareradius,5+squareradius);
    
    Sheet* source = new Sheet;
    
    source->Set(SourceA,SourceB,SourceC);
    
    
    //Lists storing output files
    
    vector<double> externalsweep_conc;
    vector<double> internalsweep_conc;
    
    for(int conc_run=1; conc_run<3; conc_run++){
        
        
        vector<double> output;
        vector<Point3D> dyeabs;
        
        vector<double> InternalEfficiency;
        double reflections;
        
        
        //Loop for each wavelength. Set wavelength Range here.
        
        double hits = 0;
        double absorbed = 0;
        double photons = 0;
        
        double conc = 1e-4*conc_run;
        
        LSC->SetConcentration(conc);
        
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
                
                double sy, sz;
                
                sy = source->GetA().y + calc->Random(1) * source->GetABLength();
                sz = source->GetA().z + calc->Random(1) * source->GetACLength();
                
                
                
                photon->SetPosition(Point3D(50,sy,sz));
                photon->SetMomentum(Vector3D(-1,0,0));
                photon->SetWavelength(wavelength);
                photon->SetRandomPolarisation();
                
                world->CorrectPhotonInside(photon);
                
                if(debug) {
                    cout<<"New photon:"<<endl<<endl;
                    print->PhotonPrint(photon);
                }
                
                while(world->ReturnPhotonInside() & photon->PhotonAliveCheck()){ //While photon is inside world and alive.
                    if(!LSC->ReturnPhotonInside()){ //If photon is not in a LSC.
                        
                        LSC->FindIntersections(*photon, debug);
                        
                        if(fulldebug){
                            cout<<"World Exit Distance is "<<world->GetInterfaceDistance(photon)<<"."<<endl;
                        }
                        
                        if(world->GetInterfaceDistance(photon)<LSC->NextDistance(*photon,fulldebug)){ //If next boundary is exit.
                            photon->PhotonKill();
                            photon->SetExit();
                            world->SetPhotonInside(0);
                            if(debug) {
                                cout<<"World exit."<<endl<<endl;
                            }
                        }
                        
                        else{
                            inout->NewCurvedIn(photon, world, *LSC, debug); //Else, entrance reflect/refract event.
                            if(matlabprint && LSC->ReturnPhotonInside()) photonpath.push_back(photon->GetPosition());
                        }
                    }
                    
                    
                    else while(LSC->ReturnPhotonInside()&&photon->PhotonAliveCheck()){ //while photon is in LSC.
                        
                        LSC->FindIntersections(*photon, debug);
                        
                        
                        if(photon->GetAbsorbLength()<=LSC->NextDistance(*photon,fulldebug)){ //If absorption = next event.
                            bool d = 0;
                            double b = 0;
                            LSC->AbsorptionEvent(photon,debug,matlabprint,dyeabs,photonpath,d,b); //Absorption event.
                        }
                        
                        else{ //If boundary is next event.
                            
                            int nextinterface = LSC->NextIntersection(*photon,fulldebug);
                            
                            if((nextinterface!=0 && nextinterface!=1)){
                                photon->PhotonKill(); //If sheet is not inside or outside. Kill photon + add counters.
                                hits++;
                                thishits++;
                                if(debug){
                                    cout<<"Hit on sheet "<<nextinterface<<" at interface point: ";
                                    print->PrintPoint(photon->GetPosition()+photon->GetMomentum()*LSC->NextDistance(*photon,fulldebug));
                                    cout<<endl;
                                }
                            }
                            else{
                                inout->NewCurvedOut(photon, *LSC, world, debug, reflections); //Otherwise exit reflect/refract event.
                            }
                        }
                    }
                }
                
                //Counters
                
                if(photon->GetAbsorptions()!=0) absorbed++;
                
                //Deletes photon.
                
                world->SetPhotonInside(0);
                LSC->SetPhotonInside(0);
                paths.push_back(photonpath);
                delete photon;
                
            }
            if(wavelengthprint){
                cout<<"Wavelength "<<wavelength<<"nm done."<<endl;
            }
            
            
        }
        
        //Calculates total efficiency and prints as 'result'
        
        double result = 100 * hits/photons;
        double internal_eff = 100 * hits/absorbed;

        externalsweep_conc.push_back(result);
        internalsweep_conc.push_back(internal_eff);
        
        if(matlabprint) matlab->DyeAbsorbPrint(dyeabs);
        cout<<"Total Optical Efficiency: "<<result<<"%"<<endl;
        cout<<"Internal Optical Efficiency: "<<internal_eff<<"%"<<endl;
        
        if(matlabprint) matlab->PhotonPathPrint(paths);
    }
        print->PrintVectorFile(internalsweep_conc,"internal.txt");
        print->PrintVectorFile(externalsweep_conc,"external.txt");
        
        internalsweep.push_back(internalsweep_conc);
        externalsweep.push_back(externalsweep_conc);
    }
    
    print->PrintMatrixFile(internalsweep, "internalsweep.txt");
    print->PrintMatrixFile(externalsweep, "externalsweep.txt");

    
    
}




int main(int argc, const char * argv[]){
    
    //double runs, int start, int end, bool matlabprint, bool debug, bool fulldebug, bool wavelengthprint
    
    //run(2000,1,350,520,0,0,1); //Simulation #1: Flat LSC
    
    //flexirun(2000, 350, 520, 0, 0, 0, 1); //Simulation #2: Flexible (Old algorithm)
    
    //flexirun_pof_new(300000, 450, 450, 0, 0,0, 1, 10); //POF Paper, new algorithm
    
    //flexirun_new(15000, 350, 520, 0, 0, 0, 1, 20); //Flexible LSC simulation
    
    //double runs, int lscs, int start, int end, bool debug, bool wavelengthprint, bool hot, double radius, double conc1, double thickness
    //hybrid(10000,1,300,2500,0,1,0,10,5e15,1); //Hybrid Model simulation.
    
    
    testsesh();
    
    //flexirun_new(15000, 350, 520, 0, 0, 0, 1, 300);
    
    //parametersweep(15000, 350, 520, 0, 0, 0, 1, 300);

    
    
}
