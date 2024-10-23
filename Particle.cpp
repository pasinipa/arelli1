#include "particle.hpp"

Particle::Particle(const std::string& name, std::array<double, 3> impulse = {0, 0, 0}) : impulse{impulse}
{
  if (indexTable.contains(name))
    index{indexTable[name]};
  // else throw ? e poi stampare a schermo il fatto che c'è stato un problema
}

static void Particle::addParticleType(const std::string& name, const ParticleType& particleType) {
  if (not indexTable.contains(name)){ 
    // push_back or emplace?
    typeTable.push_back(particleType);
    indexTable.insert({name, --typeTable.size()});
  }
  //else non fa niente oppure aggiorna la sua definizione di quella particella?
}

static void Particle::printTypeTable() {
  for (const auto& t : typeTable) {
    t.print();
    std::cout << '\n';
  }
}
  

