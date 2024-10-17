#pragma once
#include "ParticleType.hpp"

class ResonanceType : public ParticleType
{
 private:
  double width;

 public:
  ResonanceType(const std::string& name, double mass, int charge, double width);
  double getWidth() const;

  void print() const override;
};