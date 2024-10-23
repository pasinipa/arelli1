#pragma once
#include <string>

namespace pt {
class ParticleType
{
 private:
  const std::string name;
  const double mass;
  const int charge;

 public:
  explicit ParticleType(const std::string& name, double mass, int charge);
  virtual ~ParticleType() {};
  const std::string& getName() const;
  double getMass() const;
  int getCharge() const;

  virtual void print() const;
};
} // namespace pt
