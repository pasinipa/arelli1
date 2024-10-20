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
  //Pier: io terrei std::vector che a ducati l'arelli ha detto che va bene
  static std::vector<ResonanceType*> typeTable;
  static int findParticle(const std::string& name);

 public:
  Particle(const std::string& name, std::array<double, 3> impulse = {0, 0, 0});
  static void addParticleType(const std::string& name, double mass, double charge, double width = 0);     // perchè non mettiamo al posto che in std::string name, double mass e double charge invece che int? 
  int getIndex() const;

  void set_index(int);

  void set_index_by_name(const std::string&);

  static void PrintParticleTypes();
};



