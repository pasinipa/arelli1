#pragma once
#include <string>
#include <optional>

class ParticleType
{
 private:
  const std::string name;
  const double mass;
  const int charge;
  const std::optional<double> resonanceWidth;

 public:
  ParticleType(const std::string& name, double mass, int charge);
  ParticleType(const std::string& name, double mass, int charge, double width);
  double getMass() const;
  int getCharge() const;
  std::string getName() const;
  virtual void print() const;
};
