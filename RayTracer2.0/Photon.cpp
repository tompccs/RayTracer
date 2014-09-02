#include "Photon.h"

Photon::Photon():
Momentum(),
Position(),
Polarisation(),
Wavelength(),
AbsorbLength(DBL_MAX),
ScatterLength(DBL_MAX),
Alive(1),
Inside(0),
Absorptions(0),
Exit(0),
QYLoss(0)
{}

Photon::Photon(const Vector3D& M, const Point3D& P):
Momentum(M),
Position(P),
Polarisation(),
Wavelength(),
AbsorbLength(DBL_MAX),
ScatterLength(DBL_MAX),
Alive(1),
Inside(0),
Absorptions(0),
Exit(0),
QYLoss(0)
{}

Photon::Photon(const Photon& P):
Momentum(P.Momentum),
Position(P.Position),
Polarisation(P.Polarisation),
Wavelength(P.Wavelength),
ScatterLength(P.ScatterLength),
AbsorbLength(P.AbsorbLength),
Alive(P.Alive),
Inside(P.Inside),
Absorptions(P.Absorptions),
Exit(P.Exit),
QYLoss(P.QYLoss)
{}

Photon::~Photon()
{}

void
Photon::SetMomentum(const Vector3D &M){
    Vector3D a = M;
    Momentum = a.Normalise();
}

void
Photon::SetPosition(const Point3D &P){
    Position = P;
}

void
Photon::SetPolarisation(const Vector3D &P){
    Polarisation = P;
}

void
Photon::SetWavelength(const double &W){
    Wavelength = W;
}

void
Photon::SetAbsorblength(const double &P){
    AbsorbLength = P;
}

Vector3D&
Photon::GetMomentum(){
    return Momentum;
}

Point3D&
Photon::GetPosition(){
    return Position;
}

Vector3D&
Photon::GetPolarisation(){
    return Polarisation;
}

double&
Photon::GetWavelength(){
    return Wavelength;
}

double&
Photon::GetAbsorbLength(){
    return AbsorbLength;
}

void
Photon::SetRandomPolarisation(){
    
    Vector3D orthogonal;
    double min_len = 1e-3;
        
    Vector3D tmp_vec = Momentum % Vector3D(1,0,0);
    Vector3D output;
    
    if(Magnitude(tmp_vec)>min_len){
        output = Momentum % tmp_vec;
        output.Normalise();
    }
    else{
        tmp_vec = Momentum % Vector3D(0,1,0);
        output = Momentum % tmp_vec;
        output.Normalise();
    }

    Vector3D orth_vec1 = output;
    Vector3D orth_vec2 = output % orth_vec1;
        
    double phi = ((double) rand() / (RAND_MAX))*2*M_PI;
    double sinp = sin(phi);
    double cosp = cos(phi);
        
    Vector3D out_vec = orth_vec1*cosp + orth_vec2*sinp;
        
    out_vec.Normalise();
    SetPolarisation(out_vec);
}

bool&
Photon::PhotonAliveCheck(){
    return Alive;
}

void
Photon::PhotonKill(){
    Alive = 0;
}

void
Photon::SetInside(){
    Inside = 1;
}

void
Photon::Addabsorption(){
    Absorptions++;
}

bool&
Photon::GetInside(){
    return Inside;
}

int&
Photon::GetAbsorptions(){
    return Absorptions;
}

void
Photon::SetExit(){
    Exit = 1;
}

bool&
Photon::GetExit(){
    return Exit;
}

void
Photon::SetQYLoss(){
    QYLoss = 1;
}

bool&
Photon::GetQYLoss(){
    return QYLoss;
}

void
Photon::Progress(double& distance){
    Position = Position + Momentum*distance;
}