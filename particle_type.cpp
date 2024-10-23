#include "particle_type.hpp"
#include <iostream>

ParticleType::ParticleType(double mass, int charge)
    : mass{mass}
    , charge{charge}
    , resonanceWidth{}
{}

ParticleType::ParticleType(double mass, int charge, double width)
    : mass(mass)
    , charge(charge)
    , resonanceWidth(width)
{}

void ParticleType::print() const
{
  std::cout << "Name: " << name << '\n'
            << "Mass: " << mass << '\n'
            << "Charge: " << charge << '\n';
  if (resonanceWidth.has_value())
    std::cout << "Resonance Width: " << resonanceWidth.value() << '\n';
  else
    std::cout << "Resonance Width: n/a";
}

std::string ParticleType::getName() const
{
  return name;
}
double ParticleType::getMass() const
{
  return mass;
}
int ParticleType::getCharge() const
{
  return charge;
}
