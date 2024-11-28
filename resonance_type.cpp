#include "resonance_type.hpp"

lab::ResonanceType::ResonanceType(const std::string& name, double mass,
                                 int charge, double width)
    : ParticleType{name, mass, charge}
    , width_{width} {};

double lab::ResonanceType::getWidth() const
{
  return width_;
}

void lab::ResonanceType::print() const
{
  ParticleType::print();
  std::cout << "Resonance width: " << width_ << " eV" << '\n';
}