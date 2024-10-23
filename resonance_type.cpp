#include "resonance_type.hpp"
#include <iostream>

rt::ResonanceType::ResonanceType(const std::string& name, double mass,
                                 int charge, double width)
    : ParticleType{name, mass, charge}
    , width{width} {};
double rt::ResonanceType::getWidth() const
{
  return width;
}

void rt::ResonanceType::print() const
{
  pt::ParticleType::print();
  std::cout << "Resonance width: " << width << " eV" << std::endl;
}
