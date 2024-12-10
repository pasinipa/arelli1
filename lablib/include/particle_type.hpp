#pragma once
#include <iostream>
#include <string>

namespace lab {

class ParticleType
{
 public:
  ParticleType(const std::string& name, double mass, int charge);
  virtual ~ParticleType(){};
  const std::string& getName() const;
  double getMass() const;
  int getCharge() const;
  virtual double getWidth() const;
  virtual void print() const;

 private:
  const std::string name_;
  const double mass_; // expressed in GeV/c^2
  const int
      charge_; // expressed as an integer multiple of the fundamental charge e
};

} // namespace lab
