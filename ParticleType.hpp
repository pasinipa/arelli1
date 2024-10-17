#pragma once
//#include <string>


class ParticleType {    
    const char* ParticleName;
    //std::string ParticleName;
    const double Mass;
    const int Charge;
    public:
    void ReturnMass();
    void ReturnCharge();
    void ReturnParticleName();
    
    
}