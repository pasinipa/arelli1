#include "include/particle.hpp"
#include <TFile.h>
#include <TH1F.h>
#include <TRandom.h>
#include <cmath>

int main()
{
  gRandom->SetSeed();
  lab::Particle::addParticleType("Pierone+", 0.13957, +1);
  lab::Particle::addParticleType("Pierone-", 0.13957, -1);
  lab::Particle::addParticleType("Kaone+", 0.49367, +1);
  lab::Particle::addParticleType("Kaone-", 0.49367, -1);
  lab::Particle::addParticleType("Protone+", 0.93827, +1);
  lab::Particle::addParticleType("Protone-", 0.93827, -1);
  lab::Particle::addParticleType("K*", 0.89166, 0, 0.050);
  std::cout << "Particelle aggiunte e seed inizializzato." << '\n';

  std::vector<Particle> eventParticles;
  eventParticles.reserve(safeSize);

  for (int i = 0; i < numEvents; ++i)
    generateEvent(eventParticles, numParticlesPerEvent);

  TF1* f = new TF1("azimuth", "[0]*x + [1]", 0., TMath::pi * 2.);
  f->SetParameters();

  TF1* f = new TF1("polar", "[0]*x + [1]", 0., 2pi.);
  f->SetParameters()
}

const int numEvents            = 1E5;
const int numParticlesPerEvent = 100;
const int safeSize             = 120;

void generateEvent(std::vector<Particle>& eventParticles, int numParticles)
{
  eventParticles.clear();
  eventParticles.reserve(numParticles);
  for (int i = 0; i < numParticles; ++i)
    eventParticles.push_back(Particle());

  std::cout << "Generazione degli eventi completata!" << '\n';
}
