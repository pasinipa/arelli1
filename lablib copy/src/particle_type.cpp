#include "particle_type.hpp"

lab::ParticleType::ParticleType(const std::string& name, double mass,
                                int charge)
    : name_{name}
    , mass_{mass}
    , charge_{charge}
{}

const std::string& lab::ParticleType::getName() const
{
  return name_;
}

double lab::ParticleType::getMass() const
{
  return mass_;
}

int lab::ParticleType::getCharge() const
{
  return charge_;
}

double lab::ParticleType::getWidth() const
{
  return 0.;
}

void lab::ParticleType::print() const
{
  std::cout << "Name: " << name_ << '\n'
            << "Mass: " << mass_ << "kg \n"
            << "Charge: " << charge_ << " e \n";
}