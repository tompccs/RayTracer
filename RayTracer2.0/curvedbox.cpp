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
    
    inside_a = central.GetA()-W_2;
    outside_a = central.GetA()+W_2;
    
    double H_2 = (central.GetB()/central.GetA()) * W_2;
    inside_b = central.GetB()-H_2;
    outside_b = central.GetB()+H_2;
    
    insidearc.SetA(inside_a);
    insidearc.SetB(inside_b);
    outsidearc.SetA(outside_a);
    outsidearc.SetB(outside_b);
    
    insidearc.SetCentre(central.GetCentre());
    outsidearc.SetCentre(central.GetCentre());
    
    insidearc.setstartangle(central.getstartangle());
    insidearc.setendangle(central.getendangle());
   
    outsidearc.setstartangle(central.getstartangle());
    outsidearc.setendangle(central.getendangle());
    
    //Set up sheets. First build points for edge sheets.
    
    double cos_start = cos(insidearc.getstartangle());
    double cos_end = cos(insidearc.getendangle());
    double sin_start = sin(insidearc.getstartangle());
    double sin_end = sin(insidearc.getendangle());
    
    double h = central.GetCentre().x;
    double k = central.GetCentre().y;
    
    double x1_in = h + (inside_a * cos_start);
    double y1_in = k + (inside_a * sin_start);
    
    
    double x1_out = h + (outside_a * cos_start);
    double y1_out = k + (outside_a * sin_start);

    double x2_in = h + (inside_a * cos_end);
    double y2_in = k + (inside_a * sin_end);

    double x2_out = h + (outside_a * cos_end);
    double y2_out = k + (outside_a * sin_end);
    
    double z_low = central.GetCentre().z;
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
    
    Sheet start(bi_start,ti_start,bo_start);
    Sheet end(bo_end, bi_end, to_end);
    Sheet top(to_start,to_end,ti_start);
    Sheet bottom(bi_end, bi_start, bo_end);

    startsheet = start;
    endsheet = end;
    topplane = top;
    bottomplane = bottom;
    
}