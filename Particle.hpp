#pragma once
#include "particle_type.hpp"
#include <array>
#include <string>
#include <vector>

class Particle
{
 private:
  std::array<double, 3> impulse;
  int index;

  static std::vector<ParticleType> typeTable;
  static std::map<std::string, int> indexTable;

 public:
  Particle(const std::string& name, std::array<double, 3> impulse = {0., 0., 0.});

  static void addParticleType(const std::string& name, ParticleType particleType);     
  static void printTypeTable();

  // perchè non mettiamo al posto che in std::string name, double mass e double charge invece che int? 

  // Hai ragione, la massa dovrebbe essere un double. 
  // La carica invece dato che è quantizzata basta un intero. 
  // Anche perché in generale i computer sono stupidi nel fare operazioni con i numeri razionali. 
  // Per dire può succedere che se gli chiedi di fare 3.f + 2.f  lui è capace di dirti che fa 5.00000003f
  // (la 'f' dopo il numero indica che quei numeri il computer li deve interpretare come float)
  // - Paolo
  
  int getIndex() const;
  void setIndex(int);
  void setIndex(const std::string&);
};

