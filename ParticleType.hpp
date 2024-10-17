#pragma once
#include <string>

class ParticleType {    
  private:
    const std::string particleName;
    const double mass;
    const int charge;

  public:
    ParticleType(const std::string& name, double mass, int charge);
    double getMass() const;
    int getCharge() const;
    std::string getParticleName() const;
    void print() const;
};
