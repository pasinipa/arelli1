#include "Particle.hpp"
#include <cmath>
#include <cstring> // Per usare strcmp

namespace p {

// Definizione dei membri statici
std::vector<pt::ParticleType*> Particle::fParticleType;
int Particle::NParticleType = 0;

// Costruttore che inizializza una particella senza impulso specificato.
Particle::Particle(const std::string& name)
    : Particle(name, {0, 0, 0})
{}

// Costruttore che inizializza una particella con un impulso specificato.
Particle::Particle(const std::string& name, Impulse impulse)
    : P{impulse.px_, impulse.py_, impulse.pz_}
{
  index = findParticle(name);
  if (index == -1) {
    std::cout << "Particle type not found!" << std::endl;
  }
}

// Metodo privato per trovare un tipo di particella nell'array in base al nome.
int Particle::findParticle(const std::string& name)
{
  for (int i = 0; i < NParticleType; ++i) {
    if (fParticleType[i]->getName() == name) {
      return i;
    }
  }
  return -1;
}

// Restituisce l'indice del tipo di particella.
int Particle::getIndex() const
{
  return index;
}

// Imposta l'indice del tipo di particella in base a un valore intero.
void Particle::setIndex(int idx)
{
  if (idx >= 0 && idx < NParticleType) {
    index = idx;
  } else {
    std::cout << "Index out of bounds!" << std::endl;
  }
}

// Imposta l'indice del tipo di particella in base al nome.
void Particle::setIndex(const std::string& name)
{
  int idx = findParticle(name);
  if (idx != -1) {
    index = idx;
  } else {
    std::cout << "Particle type not found!" << std::endl;
  }
}

// Imposta l'impulso della particella.
void Particle::setImpulse(Impulse impulse)
{
  P[0] = impulse.px_;
  P[1] = impulse.py_;
  P[2] = impulse.pz_;
}

// Restituisce l'impulso della particella.
Impulse Particle::getImpulse() const
{
  return {P[0], P[1], P[2]};
}

// Restituisce la massa della particella.
double Particle::getMass() const
{
  return fParticleType[index]->getMass();
}

// Restituisce l'energia totale della particella.
double Particle::getEnergy() const
{
  double m         = getMass();
  double p_squared = P[0] * P[0] + P[1] * P[1] + P[2] * P[2];
  return std::sqrt(m * m + p_squared);
}

// Aggiunge un nuovo tipo di particella all'array fParticleType.
void Particle::addParticleType(const std::string& name, double mass, int charge,
                               double width)
{
  if (NParticleType < maxNumParticleType) {
    if (findParticle(name) == -1) {
      if (width == 0) {
        fParticleType.push_back(new pt::ParticleType(name, mass, charge));
      } else {
        fParticleType.push_back(
            new rt::ResonanceType(name, mass, charge, width));
      }
      ++NParticleType;
    } else {
      std::cout << "Particle type already exists!" << std::endl;
    }
  } else {
    std::cout << "Maximum number of particle types reached!" << std::endl;
  }
}

// Stampa a schermo le proprietà della particella corrente.
void Particle::Print() const
{
  if (index != -1) {
    fParticleType[index]->print();
    std::cout << "Impulse: (" << P[0] << ", " << P[1] << ", " << P[2] << ")"
              << std::endl;
  } else {
    std::cout << "Particle type not found!" << std::endl;
  }
}

// Stampa a schermo tutti i tipi di particelle definiti.
void Particle::printParticleType()
{
  for (int i = 0; i < NParticleType; ++i) {
    fParticleType[i]->print();
  }
}

} // namespace p

// #include "particle.hpp"
// #include <cmath>

// namespace p {
// int Particle::findParticle(const std::string& name) {
//   for(size_t i{0}; i != fParticleType.size(); ++i) {
//     if(name == fParticleType[i]->getName()){
//     return static_cast<int>(i);
//   }
//   }
//   return -1;
// }

// Particle::Particle(const std::string& name,
//                    std::array<double, 3> impulse = {0, 0, 0})
//     : impulse{impulse}
// {
//   if (indexTable.contains(name))
//     index{indexTable[name]};
//   // else throw ? e poi stampare a schermo il fatto che c'è stato un problema
// }

// static void Particle::addParticleType(const std::string& name,
//                                       const ParticleType& particleType)
// {
//   if (not indexTable.contains(name)) {
//     // push_back or emplace?
//     typeTable.push_back(particleType);
//     indexTable.insert({name, --typeTable.size()});
//   }
//   // else non fa niente oppure aggiorna la sua definizione di quella
//   particella?
// }

// static void Particle::printTypeTable()
// {
//   for (const auto& t : typeTable) {
//     t.print();
//     std::cout << '\n';
//   }
// }

// void Particle::printParticle()
// {
//   std::cout /*<< "Name:" << Particle::name << '\n'*/
//       << "Impulse first component:" << Particle::impulse[0] << '\n'
//       << "Impulse second component:" << Particle::impulse[1] << '\n'
//       << "Impulse third component:" << Particle::impulse[2] << '\n';
// }
// } // namespace p

// /*int Particle::calculateEnergy(){
//   energy = std::sqrt(std::pow(Particle::mass,2)
//               + std::pow(Particle::impulse[0],2)
//               + std::pow(Particle::impulse[1],2)
//               + std::pow(Particle::impulse[2],2))
// }*/

// /*static auto seed = static_cast<unsigned int>(
//     std::chrono::steady_clock::now().time_since_epoch().count());
// static std::default_random_engine eng(seed);
// static std::uniform_real_distribution<double> angleThetaDistribution(0, 360);
// static std::uniform_real_distribution<double> anglePhiDistribution(0, 360);
// static std::exponential_distribution<double> impulseModuleDistribution(0,
// MAX_IMPULSE); oppure static std::exponential_distribution<double>
// impulseComponentDistribution(0, MAX_IMPULSE);

// Particle::Particle()
//     : angTheta{angleThetaDistribution(eng))}
//     , m_velocity{velDistribution(eng), velDistribution(eng)}
//     , impulseModule{}
//     oppure
//     ,impulse{impulseComponentDistribution(eng),
// impulseComponentDistribution(eng), impulseComponentDistribution(eng)}
// {
//   bisogna o comporre l'impulso se usiamo gli angoli oppure facciamo controlli
//   su
// possibili valori troppo alti del modulo
// }

// */
