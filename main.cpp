#include "Particle.hpp"
#include "particle_type.hpp"
#include "resonance_type.hpp"
#include <TFile.h>
#include <TH1F.h>
#include <TRandom.h>
#include <iostream>
#include <vector>

int main(){
  // Inizializza il seed del generatore
  gRandom->SetSeed();

  // Aggiungo i tipi di particelle
  p::Particle::addParticleType("Pione+", 0.13957, +1);
  p::Particle::addParticleType("Pione-", 0.13957, -1);
  p::Particle::addParticleType("Kaone+", 0.49367, +1);
  p::Particle::addParticleType("Kaone-", 0.49367, -1);
  p::Particle::addParticleType("Protone+", 0.93827, +1);
  p::Particle::addParticleType("Protone-", 0.93827, -1);
  p::Particle::addParticleType("K*", 0.89166, 0, 0.050);

  std::cout << "Particelle aggiunte e seed inizializzato." << std::endl;

  void generateEvent(std::vector<Particle>& eventParticles, int numParticles) {
    eventParticles.clear();
    eventParticles.reserve(numParticles);
    for (int i = 0; i < numParticles; ++i) {
        eventParticles.push_back(Particle());
  }

    const int numEvents = 105;
    const int numParticlesPerEvent = 100;
    const int safeSize = 120;

    // Definizione del vettore di particelle all'esterno dei cicli
    std::vector<Particle> eventParticles;
    eventParticles.reserve(safeSize);

    for (int i = 0; i < numEvents; ++i) {
        generateEvent(eventParticles, numParticlesPerEvent);
    
    }

    std::cout << "Generazione degli eventi completata!" << std::endl;

  return 0;
}}
