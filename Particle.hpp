#pragma once
#include "particle_type.hpp"
#include <array>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace p {
class Particle
{
 private:
  std::array<double, 3> impulse;
  int index;

  static std::vector<ParticleType> typeTable;
  static std::map<std::string, int> indexTable;

 public:
  explicit Particle(const std::string& name,
                    std::array<double, 3> impulse = {0., 0., 0.});

  static void addParticleType(const std::string& name,
                              ParticleType particleType);
  static void printTypeTable();

  int getIndex() const;
  void setIndex(int);
  void setIndex(const std::string&);
};
} // namespace p
