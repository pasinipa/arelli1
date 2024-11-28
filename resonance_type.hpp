#pragma once
#include "particle_type.hpp"

namespace lab {

class ResonanceType : public ParticleType
{
 public:
  explicit ResonanceType(const std::string& name, double mass, int charge,
                         double width);
  double getWidth() const override;
  void print() const override;

 private:
  const double width_; // expressed in eV
};

} // namespace lab