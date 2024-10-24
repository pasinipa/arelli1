#include "Particle.hpp"
#include <cmath>
#include <cstring>

namespace p {
std::vector<pt::ParticleType*> Particle::fParticleType;
int Particle::NParticleType = 0;

Particle::Particle(const std::string& name)
    : Particle(name, {0, 0, 0})
{}

Particle::Particle(const std::string& name, Impulse impulse)
    : P{impulse.px_, impulse.py_, impulse.pz_}
{
  index = findParticle(name);
  if (index == -1) {
    std::cout << "Particle type not found!" << std::endl;
  }
}

int Particle::findParticle(const std::string& name)
{
  for (int i = 0; i < NParticleType; ++i) {
    if (fParticleType[i]->getName() == name) {
      return i;
    }
  }
  return -1;
}

int Particle::getIndex() const
{
  return index;
}

void Particle::setIndex(int idx)
{
  if (idx >= 0 && idx < NParticleType) {
    index = idx;
  } else {
    std::cout << "Index out of bounds!" << std::endl;
  }
}

void Particle::setIndex(const std::string& name)
{
  int idx = findParticle(name);
  if (idx != -1) {
    index = idx;
  } else {
    std::cout << "Particle type not found!" << std::endl;
  }
}

void Particle::setImpulse(Impulse impulse)
{
  P[0] = impulse.px_;
  P[1] = impulse.py_;
  P[2] = impulse.pz_;
}

Impulse Particle::getImpulse() const
{
  return {P[0], P[1], P[2]};
}

double Particle::getMass() const
{
  return fParticleType[index]->getMass();
}

double Particle::getEnergy() const
{
  double m         = getMass();
  double p_squared = P[0] * P[0] + P[1] * P[1] + P[2] * P[2];
  return std::sqrt(m * m + p_squared);
}

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

void Particle::printParticleType()
{
  for (int i = 0; i < NParticleType; ++i) {
    fParticleType[i]->print();
  }
}

} // namespace p

// #include "Particle.hpp"
// #include <cmath>
// #include <cstring> // Per usare strcmp

// namespace p {

// // Definizione dei membri statici
// std::vector<pt::ParticleType*> Particle::fParticleType;
// int Particle::NParticleType = 0;

// // Costruttore che inizializza una particella senza impulso specificato.
// Particle::Particle(const std::string& name)
//     : Particle(name, {0, 0, 0})
// {}

// // Costruttore che inizializza una particella con un impulso specificato.
// Particle::Particle(const std::string& name, Impulse impulse)
//     : P{impulse.px_, impulse.py_, impulse.pz_}
// {
//   index = findParticle(name);
//   if (index == -1) {
//     std::cout << "Particle type not found!" << std::endl;
//   }
// }

// // Metodo privato per trovare un tipo di particella nell'array in base al
// nome. int Particle::findParticle(const std::string& name)
// {
//   for (int i = 0; i < NParticleType; ++i) {
//     if (fParticleType[i]->getName() == name) {
//       return i;
//     }
//   }
//   return -1;
// }

// // Restituisce l'indice del tipo di particella.
// int Particle::getIndex() const
// {
//   return index;
// }

// // Imposta l'indice del tipo di particella in base a un valore intero.
// void Particle::setIndex(int idx)
// {
//   if (idx >= 0 && idx < NParticleType) {
//     index = idx;
//   } else {
//     std::cout << "Index out of bounds!" << std::endl;
//   }
// }

// // Imposta l'indice del tipo di particella in base al nome.
// void Particle::setIndex(const std::string& name)
// {
//   int idx = findParticle(name);
//   if (idx != -1) {
//     index = idx;
//   } else {
//     std::cout << "Particle type not found!" << std::endl;
//   }
// }

// // Imposta l'impulso della particella.
// void Particle::setImpulse(Impulse impulse)
// {
//   P[0] = impulse.px_;
//   P[1] = impulse.py_;
//   P[2] = impulse.pz_;
// }

// // Restituisce l'impulso della particella.
// Impulse Particle::getImpulse() const
// {
//   return {P[0], P[1], P[2]};
// }

// // Restituisce la massa della particella.
// double Particle::getMass() const
// {
//   return fParticleType[index]->getMass();
// }

// // Restituisce l'energia totale della particella.
// double Particle::getEnergy() const
// {
//   double m         = getMass();
//   double p_squared = P[0] * P[0] + P[1] * P[1] + P[2] * P[2];
//   return std::sqrt(m * m + p_squared);
// }

// // Aggiunge un nuovo tipo di particella all'array fParticleType.
// void Particle::addParticleType(const std::string& name, double mass, int
// charge,
//                                double width)
// {
//   if (NParticleType < maxNumParticleType) {
//     if (findParticle(name) == -1) {
//       if (width == 0) {
//         fParticleType.push_back(new pt::ParticleType(name, mass, charge));
//       } else {
//         fParticleType.push_back(
//             new rt::ResonanceType(name, mass, charge, width));
//       }
//       ++NParticleType;
//     } else {
//       std::cout << "Particle type already exists!" << std::endl;
//     }
//   } else {
//     std::cout << "Maximum number of particle types reached!" << std::endl;
//   }
// }

// // Stampa a schermo le proprietà della particella corrente.
// void Particle::Print() const
// {
//   if (index != -1) {
//     fParticleType[index]->print();
//     std::cout << "Impulse: (" << P[0] << ", " << P[1] << ", " << P[2] << ")"
//               << std::endl;
//   } else {
//     std::cout << "Particle type not found!" << std::endl;
//   }
// }

// // Stampa a schermo tutti i tipi di particelle definiti.
// void Particle::printParticleType()
// {
//   for (int i = 0; i < NParticleType; ++i) {
//     fParticleType[i]->print();
//   }
// }

// } // namespace p