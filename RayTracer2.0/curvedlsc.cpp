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
    
    double x1_in = h + (radius * cos_start);
    double y1_in = k + (radius * sin_start);
    
    
    double x1_out = h + (radius * cos_start);
    double y1_out = k + (radius * sin_start);
    
    double x2_in = h + (radius * cos_end);
    double y2_in = k + (radius * sin_end);
    
    double x2_out = h + (radius * cos_end);
    double y2_out = k + (radius * sin_end);
    
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

    start.OverrideNormal();
    end.OverrideNormal();
    top.OverrideNormal();
    bottom.OverrideNormal();

}