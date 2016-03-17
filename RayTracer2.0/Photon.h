#ifndef __RayTracer2_0__Photon__
#define __RayTracer2_0__Photon__

#include <iostream>
#include "Point3D.h"
#include "Point2D.h"
#include <cfloat>


class Photon{
    Vector3D Momentum;
    Point3D Position;
    Vector3D Polarisation;
    double Wavelength;
    double AbsorbLength;
    double ScatterLength;
    bool Alive;
    bool Inside;
    int Absorptions;
    bool Exit;
    bool QYLoss;
    bool err;
    
public:
    Photon();
    Photon(const Vector3D& M, const Point3D& P);
    Photon(const Photon& P);
    ~Photon();
    
    void SetMomentum(const Vector3D& M);
    void SetPosition(const Point3D& P);
    void SetPolarisation(const Vector3D& P);
    void SetWavelength(const double& W);
    void SetAbsorblength(const double& P);
    void SetScatterlength(const double& S);
    Vector3D& GetMomentum();
    Point3D& GetPosition();
    Vector3D& GetPolarisation();
    double& GetWavelength();
    double& GetAbsorbLength();
    double& GetScatterLength();
    void SetRandomPolarisation();
    bool& PhotonAliveCheck();
    void PhotonKill();
    void SetInside();
    void Addabsorption();
    bool& GetInside();
    int& GetAbsorptions();
    void SetExit();
    bool& GetExit();
    void SetQYLoss();
    bool& GetQYLoss();
    void Progress(double& distance);
    bool PointInline(Point3D& point);
    bool PointInline2D(Point2D& point);
};

#endif