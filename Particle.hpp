#pragma once
#include "ResonanceType.hpp"
#include <array>
#include <string>
#include <vector>

class Particle
{
 private:
  std::array<double, 3> impulse;
  int index;

  // potrebbe essere un std::set? e poi potremmo fare a meno di usare i puntatori...
  static std::vector<ResonanceType*> typeTable;
  static int findParticle(const std::string& name);

 public:
  Particle(const std::string& name, std::array<double, 3> impulse = {0, 0, 0});
  static void addParticleType(int name, int mass, int charge, double width = 0);
  int getIndex() const;
};
