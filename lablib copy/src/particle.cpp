#include "particle.hpp"
#include <TRandom.h>
#include <cassert>
#include <cmath>
#include <cstring>
#include <random>
#include <stdexcept>

namespace lab {

std::vector<ParticleType> Particle::particleTypeTable_;

int Particle::findParticle(const std::string& name)
{
  for (int i{0}; i < particleTypeTable_.size(); ++i) {
    if (particleTypeTable_[i].getName() == name)
      return i;
  }
  return -1;
}

/*
Particle::Particle(const std::string& name)
    : Particle(name, {0., 0., 0.})
{}

Particle::Particle(const std::string& name, Array3D&& impulse)
    : P_{impulse}
{
  typeID_ = findParticle(name);
  if (typeID_ == -1) {
    std::cout << "Particle type not found!" << '\n';
  }
  // should throw an exception here to prevent construction of an invalid object
}
*/
Particle::Particle(const char* name, Impulse impulse)
    : typeID_{findParticle(name)}
    , P_{impulse}
{}

Particle::Particle(const char* name)
    : Particle::Particle(name, {0., 0., 0.})
{}

Particle::Particle()
    : typeID_{-1}
    , P_{0., 0., 0.}
{}

/*
int Particle::decay2Body(Particle& dau1, Particle& dau2) const
{

  if (gRandom->Uniform(0., 1.) < .5) {
    dau1.setTypeID("K+");
    dau2.setTypeID("Pi-");
  } else {
    dau1.setTypeID("K-");
    dau2.setTypeID("Pi+");
  }

  if (getMass() == 0.0)
  {
    printf("Decayment cannot be performed if mass is zero\n");
    return 1;
  }

  double massMot = getMass();
  double massDau1 = dau1.getMass();
  double massDau2 = dau2.getMass();

  if (typeID_ > -1)
  { // Add width effect
    // Gaussian random numbers
    float x1, x2, w, y1;
    do
    {
      x1 = 2.0 * rand() / RAND_MAX - 1.0;
      x2 = 2.0 * rand() / RAND_MAX - 1.0;
      w = x1 * x1 + x2 * x2;
    } while (w >= 1.0);

    w = sqrt(-2.0 * log(w) / w);
    y1 = x1 * w;

    massMot += particleTypeTable_[typeID_].getWidth() * y1; // Apply width
effect
  }

  if (massMot < massDau1 + massDau2)
  {
    printf("Decayment cannot be performed because mass is too low in this
channel\n"); return 2;
  }

  double pout = sqrt(
                    (massMot + massDau1 + massDau2) * (massMot + massDau1 -
massDau2) * (massMot - massDau1 + massDau2) * (massMot - massDau1 - massDau2)) /
                massMot * 0.5;

  double norm = 2 * M_PI / RAND_MAX;

  double phi = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;
  Array3D dau1Imp = {pout * sin(theta) * cos(phi), pout * sin(theta) * sin(phi),
pout * cos(theta)}; Array3D dau2Imp = {-dau1Imp[0], -dau1Imp[1], -dau1Imp[2]};

  dau1.setImpulse(dau1Imp);
  dau2.setImpulse(dau2Imp);

  double energy = sqrt(P_[0] * P_[0] + P_[1] * P_[1] +
                       P_[2] * P_[2] + massMot * massMot);

  double bx = P_[0] / energy;
  double by = P_[1] / energy;
  double bz = P_[2] / energy;

  dau1.boost(bx, by, bz);
  dau2.boost(bx, by, bz);

  return 0;
}

void Particle::boost(double bx, double by, double bz)
{
  double energy = getEnergy();

  // Boost this Lorentz vector
  double b2 = bx * bx + by * by + bz * bz;
  double gamma = 1.0 / sqrt(1.0 - b2);
  double bp = bx * P_[0] + by * P_[1] + bz * P_[2];
  double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

  P_[0] += gamma2 * bp * bx + gamma * bx * energy;
  P_[1] += gamma2 * bp * by + gamma * by * energy;
  P_[2] += gamma2 * bp * bz + gamma * bz * energy;
}
*/

int Particle::Decay2Body(Particle& dau1, Particle& dau2) const
{
  if (particleMass() == 0.0) {
    printf("Decayment cannot be performed if mass is zero\n");
    return 1;
  }

  double massMot  = particleMass();
  double massDau1 = dau1.particleMass();
  double massDau2 = dau2.particleMass();

  if (typeID_ > -1) { // Add width effect
    // Gaussian random numbers
    float x1, x2, w, y1;
    do {
      x1 = 2.0 * rand() / RAND_MAX - 1.0;
      x2 = 2.0 * rand() / RAND_MAX - 1.0;
      w  = x1 * x1 + x2 * x2;
    } while (w >= 1.0);

    w  = sqrt(-2.0 * log(w) / w);
    y1 = x1 * w;

    massMot += 0.05 * y1; // Apply width effect
  }

  if (massMot < massDau1 + massDau2) {
    printf("Decayment cannot be performed because mass is too low in this "
           "channel\n");
    return 2;
  }

  double pout =
      sqrt((massMot + massDau1 + massDau2) * (massMot + massDau1 - massDau2)
           * (massMot - massDau1 + massDau2) * (massMot - massDau1 - massDau2))
      / massMot * 0.5;

  double norm = 2 * M_PI / RAND_MAX;

  double phi   = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;

  dau1.setImpulse({pout * sin(theta) * cos(phi), pout * sin(theta) * sin(phi),
                   pout * cos(theta)});
  dau2.setImpulse({-pout * sin(theta) * cos(phi), -pout * sin(theta) * sin(phi),
                   -pout * cos(theta)});

  double energy =
      sqrt(P_.x * P_.x + P_.y * P_.y + P_.z * P_.z + massMot * massMot);

  double bx = P_.x / energy;
  double by = P_.y / energy;
  double bz = P_.z / energy;

  dau1.Boost(bx, by, bz);
  dau2.Boost(bx, by, bz);

  return 0;
}

void Particle::Boost(double bx, double by, double bz)
{
  double energy = particleEnergy();

  // Boost this Lorentz vector
  double b2     = bx * bx + by * by + bz * bz;
  double gamma  = 1.0 / sqrt(1.0 - b2);
  double bp     = bx * P_.x + by * P_.y + bz * P_.z;
  double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

  P_.x += gamma2 * bp * bx + gamma * bx * energy;
  P_.y += gamma2 * bp * by + gamma * by * energy;
  P_.z += gamma2 * bp * bz + gamma * bz * energy;
}


void Particle::print() const
{
  if (typeID_ == -1) {
    std::cout << "Particle type not found!" << '\n';
    return;
  }
  particleTypeTable_[typeID_].print();
  std::cout << "Impulse: (" << P_.x << ", " << P_.y << ", " << P_.z << ")"
            << '\n';
}


void Particle::addParticleType(const std::string& name, double mass, int charge,
                               double width)
{
  if (findParticle(name) != -1) {
    std::cout << "Particle type already exists!" << '\n';
    return;
  }
  if (width == 0.)
    particleTypeTable_.push_back(ParticleType{name, mass, charge});
  else
    particleTypeTable_.push_back(ResonanceType{name, mass, charge, width});
}


void Particle::setTypeID(int id)
{
  if (id < 0 or id >= particleTypeTable_.size()) {
    std::cout << "Index out of bounds!" << '\n';
    return;
  }
  typeID_ = id;
}

void Particle::setTypeID(const std::string& name)
{
  int id = findParticle(name);
  if (id < 0 or id >= particleTypeTable_.size()) {
    std::cout << "Particle type not found!" << '\n';
    return;
  }
  typeID_ = id;
}

int Particle::getTypeID() const
{
  return typeID_;
}
void Particle::setImpulse(const Impulse& impulse)
{
  P_ = impulse;
}

const Impulse& Particle::getImpulse() const
{
  return P_;
}

/*
double Particle::getMass() const
{
  return particleTypeTable_[typeID_].getMass();
}
*/

double Particle::particleMass() const
{
  return particleTypeTable_[typeID_].getMass();
}

/*
double Particle::getEnergy() const
{
  double m{getMass()};
  double p_squared{P_[0] * P_[0] + P_[1] * P_[1] + P_[2] * P_[2]};
  return std::sqrt(m * m + p_squared);
}
*/
double Particle::particleEnergy() const
{
  return sqrt(std::pow(particleMass(), 2) + std::pow(normImpulse(P_), 2));
}

/*
double invariantMass(const Particle& p1, const Particle& p2)
{
  return std::sqrt(std::pow(p1.getEnergy() + p2.getEnergy(), 2)
                 - std::pow(euclidianNorm(array3DSum(p1.getImpulse(),
p2.getImpulse())), 2));
}
*/

double Particle::particleInvMass(const Particle& p) const
{
  return sqrt(std::pow(particleEnergy() + p.particleEnergy(), 2)
              - std::pow(normImpulse(sumVecImpulse(p.getImpulse(), P_)), 2));
}

} // namespace lab