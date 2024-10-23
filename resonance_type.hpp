#pragma once
#include "particle_type.hpp"

class ResonanceType : public ParticleType
{
 private:
  const double width;

 public:
  explicit ResonanceType(std::string name, double mass, int charge,
                         double width);
  double getWidth() const;

  void print() const override;
};