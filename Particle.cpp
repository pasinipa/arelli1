#include "Particle.hpp"
#include <cmath>
#include <cstring>
#include <random>
#include <stdexcept>

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

int Particle::Decay2Body(Particle& dau1, Particle& dau2) const
{
  if (getMass() == 0.0) {
    printf("Decayment cannot be preformed if mass is zero\n");
    return 1;
  }

  double massMot  = getMass();
  double massDau1 = dau1.getMass();
  double massDau2 = dau2.getMass();

  if (fIParticle > -1) { // add width effect

    // gaussian random numbers

    float x1, x2, w, y1;

    double invnum = 1. / RAND_MAX;
    do {
      x1 = 2.0 * rand() * invnum - 1.0;
      x2 = 2.0 * rand() * invnum - 1.0;
      w  = x1 * x1 + x2 * x2;
    } while (w >= 1.0);

    w  = sqrt((-2.0 * log(w)) / w);
    y1 = x1 * w;

    massMot += fParticleType[fIParticle]->GetWidth() * y1;
  }

  if (massMot < massDau1 + massDau2) {
    printf("Decayment cannot be preformed because mass is too low in this "
           "channel\n");
    return 2;
  }

  double p =
      sqrt(
          (massMot * massMot - (massDau1 + massDau2) * (massDau1 + massDau2))
          * (massMot * massMot - (massDau1 - massDau2) * (massDau1 - massDau2)))
      / massMot * 0.5;

  double norm = 2 * M_PI / RAND_MAX;

  double phi   = rand() * norm;
  double theta = rand() * norm * 0.5 - M_PI / 2.;
  dau1.setImpulse({p * std::sin(theta) * std::cos(phi),
                   p * std::sin(theta) * std::sin(phi), p * std::cos(theta)});
  dau2.setImpulse({-p * std::sin(theta) * std::cos(phi),
                   -p * std::sin(theta) * std::sin(phi), -p * std::cos(theta)});

  double energy =
      sqrt(P[0] * P[0] + P[1] * P[1] + P[2] * P[2] + massMot * massMot);

  double bx = P[0] / energy;
  double by = P[1] / energy;
  double bz = P[2] / energy;

  dau1.Boost(bx, by, bz);
  dau2.Boost(bx, by, bz);

  return 0;
}
void Particle::Boost(double bx, double by, double bz)
{
  double energy = getEnergy();

  // Boost this Lorentz vector
  double b2     = bx * bx + by * by + bz * bz;
  double gamma  = 1.0 / sqrt(1.0 - b2);
  double bp     = bx * P[0] + by * P[1] + bz * P[2];
  double gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;

  P[0] = P[0] + gamma2 * bp * bx + gamma * bx * getEnergy();
  P[1] = P[1] + gamma2 * bp * by + gamma * by * getEnergy();
  P[2] = P[2] + gamma2 * bp * bz + gamma * bz * getEnergy();
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