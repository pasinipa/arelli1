#include <iostream>
#include "particle_type.hpp"
#include "resonance_type.hpp"
#include "TRandom.h"

int mainModule() {
    gRandom->SetSeed();


    Particle::addParticleType:("Pione+",0.13957,+1);
    Particle::addParticleType:("Pione-",0.13957,-1);
    Particle::addParticleType:("Kaone+",0.49367,+1);
    Particle::addParticleType:("Kaone-",0.49637,-1);
    Particle::addParticleType:("Protone+",0.93827,+1);
    Particle::addParticleType:("Protone-",0.93827,-1);
    Particle::addParticleType:("K*",0.89166,0,0.050);

return 0;

}