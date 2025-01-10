#pragma once
#include "resonance_type.hpp"
#include <array>
#include <numeric>
#include <vector>
#include <cmath>

namespace lab {
using Array3D = std::array<double, 3>;

/*
inline double euclidianNorm(const Array3D& arr)
{
  return std::sqrt(std::inner_product(arr.begin(), arr.end(), arr.begin(), 0.));
}
inline Array3D array3DSum(const Array3D& p1, const Array3D& p2)
{
  return {p1[0] + p2[0], p1[1] + p2[1], p1[2] + p2[2]};
}
*/

inline double normImpulse(const Array3D& impulse)
{
  return std::sqrt(impulse[0]*impulse[0] + impulse[1]*impulse[1] + impulse[2]*impulse[2]);
}
inline Array3D sumVecImpulse(const Array3D &p1, const Array3D &p2)
{
  return {p1[0] + p2[0], p1[1] + p2[1], p1[2] + p2[2]};
}

class Particle
{
 public:
  Particle() = default;
  explicit Particle(const std::string&);
  explicit Particle(const std::string&, Array3D&&);

  int decay2Body(Particle& dau1, Particle& dau2) const;
  void boost(double bx, double by, double bz);
  void print() const;

  static std::vector<ParticleType> particleTypeTable_;
  static int findParticle(const std::string& name);
  static void addParticleType(const std::string&, double, int, double = 0);

  double particleInvMass(const Particle &) const;

  void setTypeID(int);
  void setTypeID(const std::string&);
  void setImpulse(const Array3D&);
  int getTypeID() const;
  const Array3D& getImpulse() const;
  double getMass() const;
  double getEnergy() const;

 private:
  int typeID_{};  // an integer code representing the kind of particle
  Array3D P_;     // the particle's momentum in cartesian coordinates   
  
};
double invariantMass (const Particle& p1, const Particle& p2);

} // namespace lab
