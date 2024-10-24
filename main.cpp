#include "Particle.hpp"
#include "TRandom.h"
#include "particle_type.hpp"
#include "resonance_type.hpp"
#include <TFile.h>
#include <TH1F.h>
#include <iostream>
#include <vector>

int main()
{
  gRandom->SetSeed();

  p::Particle::addParticleType("Pione+", 0.13957, +1);
  p::Particle::addParticleType("Pione-", 0.13957, -1);
  p::Particle::addParticleType("Kaone+", 0.49367, +1);
  p::Particle::addParticleType("Kaone-", 0.49637, -1);
  p::Particle::addParticleType("Protone+", 0.93827, +1);
  p::Particle::addParticleType("Protone-", 0.93827, -1);
  p::Particle::addParticleType("K*", 0.89166, 0, 0.050);

  TH1F* hParticleTypes =
      new TH1F("hParticleTypes", "Particle Types", 10, 0, 10);
  hParticleTypes->Sumw2();

  return 0;
  const int N = 105; // Numero di eventi
  const int M = 120; // Dimensione sicura del contenitore

  std::vector<p::Particle> eventParticles(M);

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < 100; ++j) {
      double phi   = gRandom->Uniform(0, 2 * M_PI);
      double theta = gRandom->Uniform(0, M_PI);
      double p =
          gRandom->Exp(1.0); // Distribuzione esponenziale con <p> = 1 GeV
      eventParticles[j].setImpulse({p * sin(theta) * cos(phi),
                                    p * sin(theta) * sin(phi), p * cos(theta)});
    }

    // Generazione dei tipi di particelle
    for (int j = 0; j < 100; ++j) {
      double rand = gRandom->Rndm();
      if (rand < 0.8) {
        eventParticles[j].setIndex((rand < 0.4) ? "Pione+" : "Pione-");
      } else if (rand < 0.9) {
        eventParticles[j].setIndex((rand < 0.85) ? "Kaone+" : "Kaone-");
      } else if (rand < 0.99) {
        eventParticles[j].setIndex((rand < 0.945) ? "Protone+" : "Protone-");
      } else {
        eventParticles[j].setIndex("K*");
        if (gRandom->Rndm() < 0.5) {
          eventParticles[j].Decay2Body(eventParticles[100],
                                       eventParticles[101]);
        } else {
          eventParticles[j].Decay2Body(eventParticles[102],
                                       eventParticles[103]);
        }
      }
    }

    // Riempimento dell'istogramma con i tipi di particelle
    for (const auto& particle : eventParticles) {
      hParticleTypes->Fill(particle.getIndex());
    }
  }

  // Salva gli istogrammi in un file ROOT
  TFile outFile("particle_histograms.root", "RECREATE");
  hParticleTypes->Write();
  outFile.Close();

  return 0;
}