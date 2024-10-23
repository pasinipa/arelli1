#pragma once
#include <string>
#include "ResonanceType.hpp"
#include <array>
#include <string>
#include <vector>

class Particle : public ResonanceType
{
 private:
  std::array<double, 3> impulse;
  int index;

<<<<<<< HEAD
class Particle : public ResonanceType {
    std::array <double, 3> impulse;  
    int index;
    static std::array <ResonanceType *, 10> typeTable;
    static int findParticle (const std::string &name);
    static void addParticleType(int name,int mass,int charge,double width)
    Particle(const std::string &name, std::array <double, 3> impulse);

    auto getNameArray(int index);
    auto getMassArray(int index);
    auto getChargeArray(int index);
    auto getWidthArray(int index);

    auto getImpulse(int index);

}




=======
  static std::vector<ResonanceType*> typeTable;
  static int findParticle(const std::string& name);

 public:
  Particle(const std::string& name, std::array<double, 3> impulse = {0, 0, 0});
  static void addParticleType(int name, int mass, int charge, double width = 0);
  int getIndex() const;
};
>>>>>>> 010edcb5449cc12606dc8c824c537fe09d1dd6fd
