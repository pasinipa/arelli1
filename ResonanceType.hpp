#pragma once
#include "ParticleType.hpp"

class ResonanceType : public ParticleType
{
  //   const double ResonanceLenght;
 private:
  double width;

 public:
  ResonanceType(const std::string& name, double mass, int charge, double width);
  double getWidth() const;
};
