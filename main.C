#include "particle.hpp"
#include <TMath.h>
#include <TRandom.h>
#include <TH1D.h>
#include <cmath>

const int N_EVENTS              = 1E5;
const int N_PARTICLES_PER_EVENT = 100;
const int SAFE_SIZE             = 120;

int main()
{
  gRandom->SetSeed();
  lab::Particle::addParticleType("Pione+", 0.13957, +1);
  lab::Particle::addParticleType("Pione-", 0.13957, -1);
  lab::Particle::addParticleType("Kaone+", 0.49367, +1);
  lab::Particle::addParticleType("Kaone-", 0.49367, -1);
  lab::Particle::addParticleType("Protone+", 0.93827, +1);
  lab::Particle::addParticleType("Protone-", 0.93827, -1);
  lab::Particle::addParticleType("K*", 0.89166, 0, 0.050);
  std::cout << "Particelle aggiunte e seed inizializzato." << '\n';

  std::vector<lab::Particle> eventParticles;
  std::vector<lab::Particle> daughterParticles;
  eventParticles.reserve(SAFE_SIZE);
  daughterParticles.reserve(20);

  TH1F* histoAzimuthAngle = new TH1F("phi", "Azimuthal Angle", 100, -.5, 6.7);
  TH1F* histoPolarAngle = new TH1F("theta", "Polar Angle", 100, -.5, 3.7);
  TH1I* histoTypeID = new TH1I("typeID", "TypeID", lab::Particle::particleTypeTable_.size(), -1., 10.);
  for (int i = 0; i < N_EVENTS; ++i)
    generateEvent(eventParticles, daughterParticles);

}

void generateEvent(std::vector<lab::Particle>& eventParticles, std::vector<lab::Particle>& daughterParticles)
{
  eventParticles.clear();
  daughterParticles.clear();
  double theta{};
  double phi{};
  double magnitude{};
  double percent{};

  for (int i = 0; i < N_PARTICLES_PER_EVENT; ++i) {
    theta = gRandom->Uniform(TMath::Pi());
    phi = gRandom->Uniform(2 * TMath::Pi());
    magnitude = gRandom->Exp(1.);

    // does this do a hard copy every time? should sphericalToCartesian return an r-value reference?
    lab::Array3D impulse { sphericalToCartesian (theta, phi, magnitude) };

    percent = gRandom->Uniform();
    std::string particleName;
    if (percent < .40)
      particleName = "Pione-";
    else if (percent < .80)
      particleName = "Pione+";
    else if (percent < .85)
      particleName = "Kaone-";
    else if (percent < .90)
      particleName = "Kaone+";
    else if (percent < .945)
      particleName = "Protone+";
    else if (percent < .99)
      particleName = "Protone-";
    else {
      particleName = "K*";
      lab::Particle dau1, dau2;
      if (gRandom->Uniform() < .5) {
        dau1.setTypeID("Kaone+");
        dau2.setTypeID("Pione-");
      } else {
        dau1.setTypeID("Kaone-");
        dau2.setTypeID("Pione+");
      }
      lab::Particle{particleName, impulse}.decay2Body(dau1, dau2);
      daughterParticles.push_back(dau1);
      daughterParticles.push_back(dau2);
      return;
      //it should return here right? 
      //if the particle decades we shouldn't write it into the histograms, right?
    }
    eventParticles.push_back(lab::Particle(particleName, impulse));
  }
}

lab::Array3D sphericalToCartesian(double theta, double phi, double radius) {
  double x { radius * std::sin(theta) * std::cos(phi) };
  double y { radius * std::sin(theta) * std::sin(phi) };
  double z { radius * std::cos(theta)};
  return lab::Array3D {x, y, z};
}