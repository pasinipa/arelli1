#include "ResonanceType.hpp"
#include "ParticleType.hpp"
#include <iostream>

ResonanceType::ResonanceType(const std::string& name, double mass, int charge,
                             double width)
    : ParticleType(name, mass, charge)
    , width(width)
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

double ResonanceType::getWidth() const
{
  return width;
}

void ResonanceType::print() const
{
  ParticleType::print();
  std::cout << "Width: " << width << std::endl;
}
