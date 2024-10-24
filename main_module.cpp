#include "Particle.hpp"
#include "particle_type.hpp"
#include "resonance_type.hpp"
#include <TRandom.h>
#include <iostream>

int main()
{
  gRandom->SetSeed(); // Set the seed for the random number generator

  // Aggiungi i tipi di particelle
  p::Particle::addParticleType("Pione+", 0.13957, +1);
  p::Particle::addParticleType("Pione-", 0.13957, -1);
  p::Particle::addParticleType("Kaone+", 0.49367, +1);
  p::Particle::addParticleType("Kaone-", 0.49367, -1);
  p::Particle::addParticleType("Protone+", 0.93827, +1);
  p::Particle::addParticleType("Protone-", 0.93827, -1);
  p::Particle::addParticleType("K*", 0.89166, 0, 0.050);

  return 0;
}
