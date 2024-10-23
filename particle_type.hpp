#pragma once
#include <string>

class ParticleType
{
 private:
  const std::string name;
  const double mass;
  const int charge;

 public:
  explicit ParticleType(std::string name, double mass, int charge);
  std::string getName() const;
  double getMass() const;
  int getCharge() const;

  virtual void print() const;
};