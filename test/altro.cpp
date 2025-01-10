#include "particle.hpp"
#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TH1D.h>
#include <TMath.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TStyle.h>
#include <cmath>
#include <random>

constexpr int N_EVENTS              = 40;
constexpr int N_PARTICLES_PER_EVENT = 100;
constexpr int SAFE_SIZE             = 500;

lab::Impulse sphericalToCartesian(double radius, double theta, double phi)
{
  double x{radius * std::sin(theta) * std::cos(phi)};
  double y{radius * std::sin(theta) * std::sin(phi)};
  double z{radius * std::cos(theta)};
  return {x, y, z};
}

int main()
{
  gRandom->SetSeed();
  lab::Particle::addParticleType("Pi+", 0.13957, -1, 0);
  lab::Particle::addParticleType("Pi-", 0.13957, 1, 0);
  lab::Particle::addParticleType("K+", 0.49367, -1, 0);
  lab::Particle::addParticleType("K-", 0.49367, 1, 0);
  lab::Particle::addParticleType("P+", 0.93827, -1, 0);
  lab::Particle::addParticleType("P-", 0.93827, 1, 0);
  lab::Particle::addParticleType("K*", 0.89166, 0, 0.05);

  std::vector<lab::Particle> eventParticles(SAFE_SIZE);
  lab::Particle dau1;
  lab::Particle dau2;
  TH1F* h11 = new TH1F("h11", "Invariant Mass: decayed particles", 300, 0., 1.78332);

  double magnitude{}, theta{}, phi{};
  for (int i = 0; i < N_PARTICLES_PER_EVENT; ++i) {
    magnitude = gRandom->Exp(1.);
    theta     = gRandom->Uniform(0, M_PI);
    phi       = gRandom->Uniform(0, 2 * M_PI);
    eventParticles[i].setTypeID("K*");
    eventParticles[i].setImpulse(sphericalToCartesian(magnitude, theta, phi));

    if (gRandom->Uniform(0., 1.) < .5)
    {
      dau1.setTypeID("Pi+");
      dau2.setTypeID("K-");
    }
    else
    {
      dau1.setTypeID("Pi-");
      dau2.setTypeID("K+");
    }

    eventParticles[i].Decay2Body(dau1, dau2);
    h11->Fill(dau1.particleInvMass(dau2));

    std::cout << "Invariant Mass: " << dau1.particleInvMass(dau2)  << '\n';
  }

  TFile* file = new TFile("altro.root", "RECREATE");
  h11->Write();
  file->Close();
}