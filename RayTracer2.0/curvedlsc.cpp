#include "curvedlsc.hpp"

curvedlsc::curvedlsc(){}

void
curvedlsc::Set(Point3D centre, double radius, double length, double  thickness, double height, double refractiveindex){
    
    //copy refractive indices;
    this->refractiveindex = refractiveindex;
    
    double W_2 = thickness/2;
    double inside_radius;
    double outside_radius;
    
    inside_radius = radius - W_2;
    outside_radius = radius + W_2;
    
    Point2D centre2D(centre.x,centre.y);
    
    circle inside_circle;
    inside_circle.SetCentre(centre2D);
    inside_circle.SetRadius(inside_radius);
    
    circle outside_circle;
    outside_circle.SetCentre(centre2D);
    outside_circle.SetRadius(outside_radius);
    
    tube inside_tube;
    inside_tube.SetCircle(inside_circle);
    inside_tube.SetHeight(height);
    
    tube outside_tube;
    outside_tube.SetCircle(outside_circle);
    outside_tube.SetHeight(height);
    
    inside.SetTube(inside_tube);
    outside.SetTube(outside_tube);
    
    double l = length;
    double st = -(l/(2*radius));
    double en = - st;
    
    inside.SetStart(st);
    inside.SetEnd(en);
    
    outside.SetStart(st);
    outside.SetEnd(en);
    
    double cos_start = cos(st);
    double cos_end = cos(en);
    double sin_start = sin(st);
    double sin_end = sin(en);
    
    double h = centre.x;
    double k = centre.y;
    
    double z_low = 0;
    double z_high = z_low + height;
    
    double x1_in = h + (inside_radius * cos_start);
    double y1_in = k + (inside_radius * sin_start);
    
    
    double x1_out = h + (outside_radius * cos_start);
    double y1_out = k + (outside_radius * sin_start);
    
    double x2_in = h + (inside_radius * cos_end);
    double y2_in = k + (inside_radius * sin_end);
    
    double x2_out = h + (outside_radius * cos_end);
    double y2_out = k + (outside_radius * sin_end);
    
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
    
    Sheet startsheet(bi_start,bo_start,ti_start);
    Sheet endsheet(bo_end, bi_end, to_end);
    Sheet topsheet(ti_start,to_start,ti_end);
    Sheet bottomsheet(bi_end, bo_end, bi_start);
    
    start = startsheet;
    end = endsheet;
    top = topsheet;
    bottom = bottomsheet;
    
    
    start.OverrideNormal();
    end.OverrideNormal();
    top.OverrideNormal();
    bottom.OverrideNormal();
    
}

void
curvedlsc::FindIntersections(Photon &p, bool debug){
    intersections.clear();
    
    Intersection finder_start, finder_end, finder_inside, finder_outside, finder_top, finder_bottom;
    
    finder_inside.ArchIntersect(p, inside, debug);
    intersections.push_back(finder_inside); //0
    
    finder_outside.ArchIntersect(p, outside, debug);
    intersections.push_back(finder_outside); //1
    
    finder_start.StartEndSheetIntersect(p, start, debug);
    intersections.push_back(finder_start); //2
    
    finder_end.StartEndSheetIntersect(p, end, debug);
    intersections.push_back(finder_end); //3
    
    finder_top.TopBottomSheetIntersect(p, top, debug);
    intersections.push_back(finder_top); //4
    
    finder_bottom.TopBottomSheetIntersect(p, bottom, debug);
    intersections.push_back(finder_bottom); //5
    
}

int
curvedlsc::NextIntersection(Photon &p, bool debug){
    
    double min_dist = INFINITY;
    int interface = 7;
    
    for(int i=0; i<6; i++){
        double dist = intersections[i].GetDistance();
        if((dist!=INFINITY)&&(dist<=min_dist)){
            min_dist = intersections[i].GetDistance();
            interface = i;
        }
    }
    
    return interface;
}

double
curvedlsc::NextDistance(Photon &p, bool debug){
    
    
    int NextInterface = NextIntersection(p, debug);
    double NextD = INFINITY;
    
    if (NextInterface != 7){
        NextD = intersections[NextInterface].GetDistance();
    }
    
    if(debug){
        cout<<"Next interface # is "<<NextInterface<<" and distance is "<<NextD<<endl<<endl;
    }

    
    return NextD;
}

Vector3D
curvedlsc::NextNormal(Photon &p, bool debug){
    int NextInterface = NextIntersection(p, debug);
    Vector3D NextV(NAN,NAN,NAN);
    
    if (NextInterface != 7){
        NextV = intersections[NextInterface].GetNormal();
    }
    
    return NextV;
}

double&
curvedlsc::GetRefractiveIndex(){
    return refractiveindex;
}


/*//Indicator that Material has a photon inside
void
curvedlsc::SetPhotonInside(bool inside){
    Photoninside = inside;
}

//Returns PhotonInside Indicator
bool&
curvedlsc::GetPhotonInside(){
    return Photoninside;
}*/

void
curvedlsc::PrintNextInterface(Photon &p, bool debug){
    int i = NextIntersection(p, debug);
    cout<<"Next interface number is "<<i<<endl;
    cout<<"This corresponds to interface:"<<endl;
    switch (i) {
        case 0:
            cout<<"Inside Arc"<<endl;
            break;
            
        case 1:
            cout<<"Outside Arc"<<endl;
            break;
            
        case 2:
            cout<<"Start Sheet"<<endl;
            break;
            
        case 3:
            cout<<"End Sheet"<<endl;
            break;
            
        case 4:
            cout<<"Top Sheet"<<endl;
            break;
            
        case 5:
            cout<<"Bottom Sheet"<<endl;
            break;
            
        default:
            cout<<"No intersection"<<endl;
            break;
    }
}

Sheet&
curvedlsc::GetSheet(int sheetnumber, bool debug){
    switch (sheetnumber){
        case 2:
            return start;
            break;
            
        case 3:
            return end;
            break;
            
        case 4:
            return top;
            break;
            
        case 5:
            return bottom;
            break;
    
        default:
            return start;
            break;
    }
}
