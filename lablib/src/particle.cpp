#include "particle.hpp"
#include <TRandom.h>
#include <cmath>
#include <cstring>
#include <random>
#include <stdexcept>

namespace lab {

std::vector<ParticleType> Particle::particleTypeTable_;
const int Particle::maxNumParticleTypes_{10};

int Particle::findParticle(const std::string& name)
{
  for (int i{0}; i < particleTypeTable_.size(); ++i) {
    if (particleTypeTable_[i].getName() == name)
      return i;
  }
  return -1;
}

Particle::Particle(const std::string& name)
    : Particle(name, {0., 0., 0.})
{}

Particle::Particle(const std::string& name, const Array3D& impulse)
    : P_{impulse}
{
  typeID_ = findParticle(name);
  if (typeID_ == -1) {
    std::cout << "Particle type not found!" << '\n';
  }
  // should throw an exception here to prevent construction of an invalid object
}

int Particle::decay2Body(Particle& dau1, Particle& dau2) const
{
  if (getMass() == 0.0) {
    printf("Decayment cannot be preformed if mass is zero\n");
    return 1;
  }

  if (gRandom->Uniform() < .5) {
    dau1.setTypeID("Kaone+");
    dau2.setTypeID("Pione-");
  } else {
    dau1.setTypeID("Kaone-");
    dau2.setTypeID("Pione+");
  }

  double massMot  = getMass();
  double massDau1 = dau1.getMass();
  double massDau2 = dau2.getMass();

  if (typeID_ > -1) { // add width effect

    // gaussian random numbers

    float x1, x2, w, y1;

    double invnum = 1. / RAND_MAX;
    do {
      x1 = 2.0 * rand() * invnum - 1.0;
      x2 = 2.0 * rand() * invnum - 1.0;
      w  = x1 * x1 + x2 * x2;
    } while (w >= 1.0);

    w  = sqrt((-2.0 * log(w)) / w);
    y1 = x1 * w;

    massMot += particleTypeTable_[typeID_].getWidth() * y1;
  }

  if (massMot < massDau1 + massDau2) {
    printf("Decayment cannot be preformed because mass is too low in this "
           "channel\n");
    return 2;
  }

  double p =
      sqrt(
          (massMot * massMot - (massDau1 + massDau2) * (massDau1 + massDau2))
          * (massMot * massMot - (massDau1 - massDau2) * (massDau1 - massDau2)))
      / massMot * 0.5;

  double norm = 2 * M_PI / RAND_MAX;

  double phi   = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;
  dau1.setImpulse({p * std::sin(theta) * std::cos(phi),
                   p * std::sin(theta) * std::sin(phi), p * std::cos(theta)});
  dau2.setImpulse({-p * std::sin(theta) * std::cos(phi),
                   -p * std::sin(theta) * std::sin(phi), -p * std::cos(theta)});

  double energy =
      sqrt(P_[0] * P_[0] + P_[1] * P_[1] + P_[2] * P_[2] + massMot * massMot);

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
  double b2     = bx * bx + by * by + bz * bz;
  double gamma  = 1.0 / sqrt(1.0 - b2);
  double bp     = bx * P_[0] + by * P_[1] + bz * P_[2];
  double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

  P_[0] += gamma2 * bp * bx + gamma * bx * getEnergy();
  P_[1] += gamma2 * bp * by + gamma * by * getEnergy();
  P_[2] += gamma2 * bp * bz + gamma * bz * getEnergy();
}

void Particle::print() const
{
  if (typeID_ == -1) {
    std::cout << "Particle type not found!" << '\n';
    return;
  }
  particleTypeTable_[typeID_].print();
  std::cout << "Impulse: (" << P_[0] << ", " << P_[1] << ", " << P_[2] << ")"
            << '\n';
}

void Particle::addParticleType(const std::string& name, double mass, int charge,
                               double width)
{
  if (particleTypeTable_.size() >= maxNumParticleTypes_) {
    std::cout << "Maximum number of particle types reached!" << '\n';
    return;
  }
  if (findParticle(name) != -1) {
    std::cout << "Particle type already exists!" << '\n';
    return;
  }
  if (width == 0.)
    particleTypeTable_.push_back(ParticleType{name, mass, charge});
  else
    particleTypeTable_.push_back(ResonanceType{name, mass, charge, width});
}

void Particle::printParticleTypes()
{
  for (const auto& t : particleTypeTable_)
    t.print();
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
  if (id == -1) {
    std::cout << "Particle type not found!" << '\n';
    return;
  }
  typeID_ = id;
}

void Particle::setImpulse(const Array3D& impulse)
{
  P_ = impulse;
}

int Particle::getTypeID() const
{
  return typeID_;
}

const Array3D& Particle::getImpulse() const
{
  return P_;
}

double Particle::getMass() const
{
  return particleTypeTable_[typeID_].getMass();
}

double Particle::getEnergy() const
{
  double m{getMass()};
  double p_squared{P_[0] * P_[0] + P_[1] * P_[1] + P_[2] * P_[2]};
  return std::sqrt(m * m + p_squared);
}

double invariantMass(const Particle& p1, const Particle& p2)
{
  return std::sqrt(
      std::pow(p1.getEnergy() + p2.getEnergy(), 2)
      - std::pow(euclidianNorm(array3DSum(p1.getImpulse(), p2.getImpulse())),
                 2));
}

} // namespace lab