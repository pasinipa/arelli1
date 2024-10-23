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
};

inline double normImpulse(Impulse impulse)
{
  return std::sqrt(impulse.px_ * impulse.px_ + impulse.py_ * impulse.py_
                   + impulse.pz_ * impulse.pz_);
}

inline Impulse sumVecImpulse(const Impulse& p1, const Impulse& p2)
{
  return Impulse{p1.px_ + p2.px_, p1.py_ + p2.py_, p1.pz_ + p2.pz_};
}

class Particle
{
 private:
  static const int maxNumParticleType{10};
  static std::vector<pt::ParticleType*> fParticleType;
  static int NParticleType;
  int index;
  std::array<double, 3> P;

  static int findParticle(const std::string& name);

 public:
  explicit Particle(const std::string&);
  explicit Particle(const std::string&, Impulse);

  const int getIndex() const;
  void setIndex(int);
  void setIndex(const std::string&);

  const Impulse& getImpulse() const;
  void setImpulse(Impulse);

  static void addParticleType(const std::string&, double, int, double = 0);
  static void printParticleType();

  void Print() const;
  const std::array<double, 3>& get_pulse() const;
};
} // namespace p

// #include "particle_type.hpp"
// #include <array>
// #include <iostream>
// #include <map>
// #include <string>
// #include <vector>

// namespace p {
// class Particle
// {
//  private:
//   std::array<double, 3> impulse;
//   int index;
//   int energy;

//   static std::vector<ParticleType> typeTable;
//   static std::map<std::string, int> indexTable;
//     //auto getNameArray(int index);
//     //auto getMassArray(int index);
//     //auto getChargeArray(int index);
//     //auto getWidthArray(int index);
//     //auto getImpulse(int index);

//  public:
//   explicit Particle(const std::string& name,
//                     std::array<double, 3> impulse = {0., 0., 0.});

//   static void addParticleType(const std::string& name,
//                               ParticleType particleType);
//   static void printTypeTable();

//   int getIndex() const;
//   void setIndex(int); //controllo cosa c'è alla posizione 3 e se voglio gli
//   cambio l'index void setIndex(const std::string&); //cerco se il nome è già
//   presente e se voglio gli cambio l'index
//   //int calculateEnergy();
//   //int calculateInvariantMass();
//   //int findParticle(std::string name);
//   //void const printParticle();
// };
// } // namespace p
