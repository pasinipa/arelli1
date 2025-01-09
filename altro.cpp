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

constexpr int N_EVENTS              = 1E5;
constexpr int N_PARTICLES_PER_EVENT = 100;
constexpr int SAFE_SIZE             = 120;

lab::Array3D sphericalToCartesian(double radius, double theta, double phi)
{
  double x{radius * std::sin(theta) * std::cos(phi)};
  double y{radius * std::sin(theta) * std::sin(phi)};
  double z{radius * std::cos(theta)};
  return {x, y, z};
}

int main()
{
  gRandom->SetSeed();
  lab::Particle::addParticleType("Pi+", 0.13957, 1);
  lab::Particle::addParticleType("Pi-", 0.13957, -1);
  lab::Particle::addParticleType("K+", 0.49367, 1);
  lab::Particle::addParticleType("K-", 0.49367, -1);
  lab::Particle::addParticleType("K*", 0.89166, 0, 0.050);
  TH1* h11 = new TH1F("h11", "Invariant Mass (true decayment particles)", 300,
                      0.6, 1.2);

  TFile* outputFile = new TFile("altro.root", "RECREATE");

  double magnitude{}, theta{}, phi{};
  lab::Particle particle, dau1, dau2;

  for (int i = 0; i < N_EVENTS; ++i) {
    magnitude = gRandom->Exp(1.);
    theta     = gRandom->Uniform(0, M_PI);
    phi       = gRandom->Uniform(0, 2*M_PI);
    particle.setTypeID("K*");
    particle.setImpulse(sphericalToCartesian(magnitude, theta, phi));
    particle.decay2Body(dau1, dau2);
    h11->Fill(lab::invariantMass(dau1, dau2));
  }

  h11->Write();
  outputFile->Close();
}