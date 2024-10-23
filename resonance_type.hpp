#pragma once
#include "particle_type.hpp"

namespace rt {
class ResonanceType : public pt::ParticleType
{
 private:
  const double width;

 public:
  explicit ResonanceType(const std::string& name, double mass, int charge,
                         double width);
  double getWidth() const;

  void print() const override;
};
} // namespace rt