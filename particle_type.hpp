#pragma once
#include <string>
#include <optional>

class ParticleType
{
 private:
  const std::string name;
  const double mass;
  const int charge;
  const std::optional<double> resonanceWidth;  //regaz avevavo detto che non facevamo con optional perchè lo mettiamo a zero il valore delle width per le nn resonance

 public:
  ParticleType(double mass, int charge);
  ParticleType(double mass, int charge, double width);

  virtual void print() const;

  double getMass() const;
  int getCharge() const;
  std::string getName() const;
};
