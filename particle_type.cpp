#include "particle_type.hpp"
#include <iostream>

pt::ParticleType::ParticleType(std::string& name, double mass, int charge)
    : name{name}
    , mass{mass}
    , charge{charge}
{}

const std::string& pt::ParticleType::getName() const
{
  return name;
}

double pt::ParticleType::getMass() const
{
  return mass;
}

int pt::ParticleType::getCharge() const
{
  return charge;
}

void pt::ParticleType::print() const
{
  std::cout << "Name: " << name << '\n'
            << std::endl
            << "Mass: " << mass << "kg \n"
            << std::endl
            << "Charge: " << charge << " e \n"
            << std::endl;
}