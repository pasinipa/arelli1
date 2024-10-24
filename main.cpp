#include "Particle.hpp"
#include "particle_type.hpp"
#include "resonance_type.hpp"
#include <TFile.h>
#include <TH1F.h>
#include <TRandom.h>
#include <iostream>
#include <vector>

int main()
{
  // Inizializza il seed del generatore
  gRandom->SetSeed();

  // Aggiungi i tipi di particelle
  p::Particle::addParticleType("Pione+", 0.13957, +1);
  p::Particle::addParticleType("Pione-", 0.13957, -1);
  p::Particle::addParticleType("Kaone+", 0.49367, +1);
  p::Particle::addParticleType("Kaone-", 0.49367, -1);
  p::Particle::addParticleType("Protone+", 0.93827, +1);
  p::Particle::addParticleType("Protone-", 0.93827, -1);
  p::Particle::addParticleType("K*", 0.89166, 0, 0.050);

  std::cout << "Particelle aggiunte e seed inizializzato." << std::endl;

  return 0;
}
