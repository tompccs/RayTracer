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
{
    Momentum = Momentum.Normalise();
}

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


void //Sets Photon momentum vector
Photon::SetMomentum(const Vector3D &M){
    Vector3D a = M;
    Momentum = a.Normalise();
}

void //Sets Photon Position vector
Photon::SetPosition(const Point3D &P){
    Position = P;
}

void //SetPolarisation Vector
Photon::SetPolarisation(const Vector3D &P){
    Polarisation = P;
}

void //Set Wavelength
Photon::SetWavelength(const double &W){
    Wavelength = W;
}

void // Sets Absorption Length value
Photon::SetAbsorblength(const double &P){
    AbsorbLength = P;
}

Vector3D& //Get Momentum Vector
Photon::GetMomentum(){
    return Momentum;
}

Point3D& //Get Position Vector
Photon::GetPosition(){
    return Position;
}

Vector3D& //Get Polarisation Vector
Photon::GetPolarisation(){
    return Polarisation;
}

double& //Get Wavelength
Photon::GetWavelength(){
    return Wavelength;
}

double& //Get Aborption Length
Photon::GetAbsorbLength(){
    return AbsorbLength;
}

void //Set a random polarisation vectr
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

bool& //Check photon is alive
Photon::PhotonAliveCheck(){
    return Alive;
}

void //Kill Photon
Photon::PhotonKill(){
    Alive = 0;
}

void //Set Photon is inside?
Photon::SetInside(){
    Inside = 1;
}

void //Add photon absorption
Photon::Addabsorption(){
    Absorptions++;
}

bool& //Return inside value
Photon::GetInside(){
    return Inside;
}

int& //Get photon absorptions
Photon::GetAbsorptions(){
    return Absorptions;
}

void //Set photon leaving material
Photon::SetExit(){
    Exit = 1;
}

bool& //Return Exit bool
Photon::GetExit(){
    return Exit;
}

void //Set if photon has been killed by QY
Photon::SetQYLoss(){
    QYLoss = 1;
}

bool& //Get if photon has been killed by QY
Photon::GetQYLoss(){
    return QYLoss;
}

void //Progress photon by distance
Photon::Progress(double& distance){
    Position = Position + Momentum*distance;
}

bool //Check that point is on the line with the direction of the Photon
Photon::PointInline(Point3D &point){
    Point3D P0 = Position;
    Point3D P1 = Position + Momentum;
    
    // if 3 points are in line, the sums of the two distances between adjacent points = distance between farthest
    
    if(fabs(P0.distancetopoint(P1) + P1.distancetopoint(point) - P0.distancetopoint(point))<1e-6){
        return true;
    }
    
    else if (fabs(P0.distancetopoint(point) + P1.distancetopoint(point) - P0.distancetopoint(P1))<1e-6){
        return true;
    }
    return false;
}

bool //Check that point is on the line with the direction of the Photon (in 2D)
Photon::PointInline2D(Point2D &point){
    Point2D P0(Position.x, Position.y);
    Point2D P1(Position.x + Momentum.x, Position.y + Momentum.y);
    
    // if 3 points are in line, the sums of the two distances between adjacent points = distance between farthest
    
    /*cout<<"P0->P1= "<<P0.distancetopoint(P1)<<endl;
     cout<<"P1->PX= "<<P1.distancetopoint(point)<<endl;
     cout<<"P0->PX= "<<P0.distancetopoint(point)<<endl;
     */
    
    
    if(fabs(P0.distancetopoint(P1) + P1.distancetopoint(point) - P0.distancetopoint(point))<=1e-6){
        
        return true;
    }
    
    else if (fabs(P0.distancetopoint(point) + P1.distancetopoint(point) - P0.distancetopoint(P1))<=1e-6){
        return true;
    }
    
    else if (fabs(P0.distancetopoint(P1) + P0.distancetopoint(point) - P1.distancetopoint(point))<=1e-6){
        return true;
    }
    return false;
}