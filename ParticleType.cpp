#include "ParticleType.hpp"
#include <iostream>

ParticleType::ParticleType(const std::string& name, double mass, int charge)
    : particleName(name)
    , mass(mass)
    , charge(charge)
{}

std::string ParticleType::getParticleName() const
{
  return particleName;
}
double ParticleType::getMass() const
{
  return mass;
}
int ParticleType::getCharge() const
{
  return charge;
}

void ParticleType::print() const
{
  std::cout << "Name: " << particleName << ".\n " << "Mass: " << mass << ".\n"
            << "Charge: " << charge << std::endl;
}
