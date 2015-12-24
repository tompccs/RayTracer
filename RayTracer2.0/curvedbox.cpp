#include "curvedbox.hpp"
#include "arc.hpp"
#include "Sheet.h"

curvedbox::curvedbox():
insidearc(), outsidearc(), startsheet(), endsheet(), topplane(), bottomplane(), refractiveindex()
{}

curvedbox::curvedbox(arc& central, double height, double width, double refractiveindex){
    
    //copy refractive indeces
    
    this->refractiveindex = refractiveindex;
    
    //First build arcs for both inside plane and outside plane.
    
    double W_2 = width/2;
    double inside_a;
    double inside_b;
    double outside_a;
    double outside_b;
    
    inside_a = central.GetE().GetA()-W_2;
    outside_a = central.GetE().GetA()+W_2;
    
    double H_2 = (central.GetE().GetB()/central.GetE().GetA()) * W_2;
    inside_b = central.GetE().GetB()-H_2;
    outside_b = central.GetE().GetB()+H_2;
    
    insidearc.GetE().SetA(inside_a);
    insidearc.GetE().SetB(inside_b);
    outsidearc.GetE().SetA(outside_a);
    outsidearc.GetE().SetB(outside_b);
    
    insidearc.GetE().SetCentre(central.GetE().GetCentre());
    outsidearc.GetE().SetCentre(central.GetE().GetCentre());
    
    insidearc.setstartangle(central.getstartangle());
    insidearc.setendangle(central.getendangle());
    
    outsidearc.setstartangle(central.getstartangle());
    outsidearc.setendangle(central.getendangle());
    
    //Set up sheets. First build points for edge sheets.
    
    double cos_start = cos(insidearc.getstartangle());
    double cos_end = cos(insidearc.getendangle());
    double sin_start = sin(insidearc.getstartangle());
    double sin_end = sin(insidearc.getendangle());
    
    double h = central.GetE().GetCentre().x;
    double k = central.GetE().GetCentre().y;
    
    double x1_in = h + (inside_a * cos_start);
    double y1_in = k + (inside_a * fabs(sin_start));
    
    
    double x1_out = h + (outside_a * cos_start);
    double y1_out = k + (outside_a * fabs(sin_start));
    
    double x2_in = h + (inside_a * cos_end);
    double y2_in = k - (inside_a * sin_end);
    
    double x2_out = h + (outside_a * cos_end);
    double y2_out = k - (outside_a * sin_end);
    
    double z_low = central.GetE().GetCentre().z;
    double z_high = z_low + height;
    
    Point3D bi_start(x1_in,y1_in,z_low);
    Point3D ti_start(x1_in,y1_in,z_high);
    
    Point3D bi_end(x2_in,y2_in,z_low);
    Point3D ti_end(x2_in,y2_in,z_high);
    
    Point3D bo_start(x1_out,y1_out,z_low);
    Point3D to_start(x1_out,y1_out,z_high);
    
    Point3D bo_end(x2_out,y2_out,z_low);
    Point3D to_end(x2_out,y2_out,z_high);
    
    //Build (rectangular) side sheets. Note that top and bottom planes will be used as infinite planes. Distance to intersection
    //needs to be calculated carefully baring this in mind.
    
    Sheet start(bi_start,bo_start,ti_start);
    Sheet end(bo_end, bi_end, to_end);
    Sheet top(ti_start,to_start,ti_end);
    Sheet bottom(bi_end, bo_end, bi_start);
    
    startsheet = start;
    endsheet = end;
    topplane = top;
    bottomplane = bottom;
    
    startsheet.OverrideNormal();
    endsheet.OverrideNormal();
    topplane.OverrideNormal();
    bottomplane.OverrideNormal();
    
}

double
curvedbox::DToStartSheet(Photon& photon){
    
    if(startsheet.GetIntersectionTest(&photon)&&PhotonInside){
        return startsheet.IntersectionDistance(&photon);
    }
    
    return INFINITY;
}

double
curvedbox::DToEndSheet(Photon& photon){
    
    if(endsheet.GetIntersectionTest(&photon)&&PhotonInside){
        return endsheet.IntersectionDistance(&photon);
    }
    
    return INFINITY;
}

double
curvedbox::DToInsideArc(Photon &photon, bool debug){
    bool value = 0;
    
    if(debug){
        cout<<"Check for collisions on Inside Arc"<<endl;
    }
    
    if(insidearc.photonarcintersect(photon, debug)){
        value = 1;
    }
    
    if(debug){
        cout<<"Photon-arc intersect inside arc value ="<<value<<endl;
        cout<<endl;
    }
    
    if(value ==1){
        double distance = insidearc.IntersectDistance(photon, debug);
        if(distance >= 1e-9){
            return distance;
        }
    }
    
    
    return INFINITY;
}


double
curvedbox::DToOutsideArc(Photon &photon, bool debug){
    bool test = 0;
    
    if(debug){
        cout<<"Check for collisions on Outside Arc"<<endl;
    }
    
    if(outsidearc.photonarcintersect(photon, debug)){
        test = 1;
    }
    
    if(debug){
        cout<<"Photon-arc intersect outside arc value ="<<test<<endl;
        cout<<endl;
    }

    
    if(test ==1){
        double distance = outsidearc.IntersectDistance(photon, debug);
        if(distance >= 1e-9){
            return distance;
        }
    }
    
    return INFINITY;
}

double
curvedbox::DToTopSheet(Photon& photon){
    double intdistance = INFINITY;
    
    if(PhotonInside){
        intdistance = topplane.IntersectionDistance(&photon);
    }
    
    return intdistance;
}

double
curvedbox::DToBottomSheet(Photon& photon){
    
    double intdistance = INFINITY;
    
    if(PhotonInside){
        intdistance = bottomplane.IntersectionDistance(&photon);
    }
    
    return intdistance;
}


bool //checks if a point lies on the curved 2d thick arc shape defined at the top or bottom of an LSC.
curvedbox::TopBottomSheetPointCheck(Point3D& point){
    
    //cout<<endl<<endl<<"Checking if a point lies on the top or bottom sheets: "<<endl;
    
    Test reader;
    reader.PrintPoint(point);
    
    
    
    bool test = 0;
    //bool test2 = 0;
    //bool test3 = 0;
    bool finaltest = 0;
    
    
    if(outsidearc.GetE().PointInsideEllipse(point)){
        test = 1;
    }
    
    /*if(!insidearc.GetE().PointInsideEllipse(point)){
        test2 = 1;
    }
    
    double Y = fabs(insidearc.GetE().GetCentre().y - point.y);
    double X = fabs(insidearc.GetE().GetCentre().x - point.x);
    
    double angle = 0;
    
    if(X!=0){
        angle = atan(Y/X);
    }
    
    if(X==0){
        angle = M_PI_2;
    }
    
    if(angle > insidearc.getstartangle() && angle< insidearc.getendangle()){
        test3 = 1;
    }*/
    
    if(test){
        finaltest = 1;
    }
    
    
    return finaltest;
}

void
curvedbox::Set(arc central, double height, double width, double refractiveindex){
    //copy refractive indeces
    
    this->refractiveindex = refractiveindex;
    
    //First build arcs for both inside plane and outside plane.
    
    double W_2 = width/2;
    double inside_a;
    double inside_b;
    double outside_a;
    double outside_b;
    
    inside_a = central.GetE().GetA()-W_2;
    outside_a = central.GetE().GetA()+W_2;
    
    double H_2 = (central.GetE().GetB()/central.GetE().GetA()) * W_2;
    inside_b = central.GetE().GetB()-H_2;
    outside_b = central.GetE().GetB()+H_2;
    
    insidearc.GetE().SetA(inside_a);
    insidearc.GetE().SetB(inside_b);
    outsidearc.GetE().SetA(outside_a);
    outsidearc.GetE().SetB(outside_b);
    
    insidearc.GetE().SetCentre(central.GetE().GetCentre());
    outsidearc.GetE().SetCentre(central.GetE().GetCentre());
    
    insidearc.setstartangle(central.getstartangle());
    insidearc.setendangle(central.getendangle());
    
    outsidearc.setstartangle(central.getstartangle());
    outsidearc.setendangle(central.getendangle());
    
    //Set up sheets. First build points for edge sheets.
    
    double cos_start = cos(insidearc.getstartangle());
    double cos_end = cos(insidearc.getendangle());
    double sin_start = sin(insidearc.getstartangle());
    double sin_end = sin(insidearc.getendangle());
    
    double h = central.GetE().GetCentre().x;
    double k = central.GetE().GetCentre().y;
    
    double x1_in = h + (inside_a * cos_start);
    double y1_in = k + (inside_a * fabs(sin_start));
    
    
    double x1_out = h + (outside_a * cos_start);
    double y1_out = k + (outside_a * fabs(sin_start));
    
    double x2_in = h + (inside_a * cos_end);
    double y2_in = k - (inside_a * sin_end);
    
    double x2_out = h + (outside_a * cos_end);
    double y2_out = k - (outside_a * sin_end);
    
    double z_low = central.GetE().GetCentre().z;
    double z_high = z_low + height;
    
    Point3D bi_start(x1_in,y1_in,z_low);
    Point3D ti_start(x1_in,y1_in,z_high);
    
    Point3D bi_end(x2_in,y2_in,z_low);
    Point3D ti_end(x2_in,y2_in,z_high);
    
    Point3D bo_start(x1_out,y1_out,z_low);
    Point3D to_start(x1_out,y1_out,z_high);
    
    Point3D bo_end(x2_out,y2_out,z_low);
    Point3D to_end(x2_out,y2_out,z_high);
    
    //Build (rectangular) side sheets. Note that top and bottom planes will be used as infinite planes. Distance to intersection
    //needs to be calculated carefully baring this in mind.
    
    Sheet start(bi_start,bo_start,ti_start);
    Sheet end(bo_end, bi_end, to_end);
    Sheet top(ti_start,to_start,ti_end);
    Sheet bottom(bi_end, bo_end, bi_start);
    
    startsheet = start;
    endsheet = end;
    topplane = top;
    bottomplane = bottom;
    
    startsheet.OverrideNormal();
    endsheet.OverrideNormal();
    topplane.OverrideNormal();
    bottomplane.OverrideNormal();
}

int
curvedbox::NextInterface(Photon &photon, bool debug){
    
    if(debug){
        cout<<"Running NextInterface method."<<endl;
    }
    
    NextInterfaceValue = 7;
    
    NextInterfaceDistance(photon, debug);
    
    if(debug){
        cout<<"Returning sheetnumber "<<NextInterfaceValue<<endl<<endl;
    }
    
    return NextInterfaceValue;
    
    
}

double
curvedbox::NextInterfaceDistance(Photon &photon, bool debug){
    
    if(debug){
        cout<<endl;
        cout<<"Running NextInterfaceDistance method."<<endl;
    }
    
    double nextdistance = INFINITY;
    int sheetnumber = 7;
    
    double d[6];
    
    d[0] = DToStartSheet(photon);
    d[1] = DToEndSheet(photon);
    d[2] = DToInsideArc(photon, debug);
    d[3] = DToOutsideArc(photon, debug);
    d[4] = DToTopSheet(photon);
    d[5] = DToBottomSheet(photon);
    
    if(debug){
        for (int i = 0; i<6; i++){
            cout<<"Distance to sheet "<<i<<" is "<<d[i]<<endl;
        }
    }
    
    for(int i = 0; i<6; i++){
        if(d[i] != INFINITY && d[i]<nextdistance){
            nextdistance = d[i];
            sheetnumber = i;
        }
    }
    
    NextInterfaceValue = sheetnumber;
    
    if(debug){
        cout<<"Shortest distance is "<<nextdistance<<endl<<endl;
    }
            
    return nextdistance;
}

Vector3D
curvedbox::GetNextNormal(Photon &photon, bool debug){
    
    int sheetnumber = NextInterface(photon, debug);
    
    switch(sheetnumber){
        case 0:
            return startsheet.GetNormal();
            break;
        case 1:
            return endsheet.GetNormal();
            break;
        case 2:
            return insidearc.GetNormalVector(photon, debug);
            break;
        case 3:
            return outsidearc.GetNormalVector(photon, debug);
            break;
        case 4:
            return topplane.GetNormal();
            break;
        case 5:
            return bottomplane.GetNormal();
            break;
    }
    
    return Vector3D(NAN,NAN,NAN);
}

double&
curvedbox::GetRefractiveIndex(){
    return refractiveindex;
}


//Indicator that Material has a photon inside
void
curvedbox::SetPhotonInside(bool inside){
    PhotonInside = inside;
}

//Returns PhotonInside Indicator
bool
curvedbox::GetPhotonInside(){
    return PhotonInside;
}

Sheet&
curvedbox::GetStartSheet(){
    return startsheet;
}

Sheet&
curvedbox::GetEndSheet(){
    return endsheet;
}
