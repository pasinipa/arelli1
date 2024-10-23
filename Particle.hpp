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
  int energy;

  static std::vector<ParticleType> typeTable;
  static std::map<std::string, int> indexTable;
    //auto getNameArray(int index);
    //auto getMassArray(int index);
    //auto getChargeArray(int index);
    //auto getWidthArray(int index);
    //auto getImpulse(int index);

 public:
  explicit Particle(const std::string& name,
                    std::array<double, 3> impulse = {0., 0., 0.});

  static void addParticleType(const std::string& name,
                              ParticleType particleType);
  static void printTypeTable();

  int getIndex() const;
  void setIndex(int); //controllo cosa c'è alla posizione 3 e se voglio gli cambio l'index
  void setIndex(const std::string&); //cerco se il nome è già presente e se voglio gli cambio l'index
  //int calculateEnergy();
  //int calculateInvariantMass();
  //int findParticle(std::string name);
  //void const printParticle(); 
};
} // namespace p
