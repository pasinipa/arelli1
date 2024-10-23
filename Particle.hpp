#pragma once
#include "resonance_type.hpp"
#include <array>
#include <iostream>
#include <vector>

namespace p {
struct Impulse
{
  double px_;
  double py_;
  double pz_;
}; // Impulso rappresenta un vettore 3D delle componenti dell'impulso di una
   // particella.

inline double normImpulse(
    Impulse impulse) // normImpulse calcola la norma del vettore impulso.
{
  return std::sqrt(impulse.px_ * impulse.px_ + impulse.py_ * impulse.py_
                   + impulse.pz_ * impulse.pz_);
}

inline Impulse
sumVecImpulse(const Impulse& p1,
              const Impulse& p2) // sumVecImpulse calcola la somma vettoriale di
                                 // due impulsi.
{
  return Impulse{p1.px_ + p2.px_, p1.py_ + p2.py_, p1.pz_ + p2.pz_};
}

class Particle
{
 private:
  static const int maxNumParticleType{10};
  // maxNumParticleType è il numero massimo di tipi di particelle che possiamo
  // gestire.
  static std::vector<pt::ParticleType*> fParticleType;
  // ParticleType è un vector di puntatori a tipi di particelle.
  static int NParticleType;
  // NParticleType tiene traccia del numero di tipi di particelle attualmente
  // definiti.
  int index;
  // index è l'indice del tipo di particella corrente nell'array ParticleType.
  std::array<double, 3> P;
  // P è un array che contiene le componenti dell'impulso della particella.

  static int findParticle(const std::string& name);
  // indParticle trova l'indice di un tipo di particella nell'array ParticleType
  // in base al nome.

 public:
  explicit Particle(const std::string&);
  explicit Particle(const std::string&, Impulse);
  // Costruttori: creano una particella e impostano il suo tipo e impulso.

  const int getIndex() const;
  // getIndex restituisce l'indice del tipo di particella.
  void setIndex(int);
  // setIndex imposta l'indice del tipo di particella in base a un valore
  // intero.
  void setIndex(const std::string&);
  // setIndex imposta l'indice del tipo di particella in base al nome.

  const Impulse& getImpulse() const;
  // getImpulse restituisce l'impulso della particella.
  void setImpulse(Impulse);
  // setImpulse imposta l'impulso della particella.

  static void addParticleType(const std::string&, double, int, double = 0);
  // addParticleType aggiunge un nuovo tipo di particella all'array
  // ParticleType.
  static void printParticleType();
  // printParticleTypes stampa a schermo tutti i tipi di particelle definiti.

  void Print() const;
  // print stampa a schermo le proprietà della particella corrente.
  const std::array<double, 3>& get_pulse() const;
  // get_pulse restituisce le componenti dell'impulso.
};
} // namespace p
