#pragma once
#include <string>



class Particle : public ResonceType {
    std::array <double, 3> impulse;  
    int index;
    static int findParticle (const std::string &name);
    static std::array <ResonanceType *, 10> typeTable;
    static void addParticleType(int name,int mass,int charge,double width)

    Particle(const std::string &name, std::array <double, 3> impulse);
}