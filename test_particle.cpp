#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Particle.hpp"
#include "doctest.h"

TEST_CASE("ParticleType class tests")
{
  lab::ParticleType electron("Electron", 9.10938356e-31, -1);

  CHECK(electron.getName() == "Electron");
  CHECK(electron.getMass() == doctest::Approx(9.10938356e-31));
  CHECK(electron.getCharge() == -1);
}

TEST_CASE("ResonanceType class tests")
{
  lab::ResonanceType resonance("Resonance", 1.6726219e-27, 1, 0.1);

  CHECK(resonance.getName() == "Resonance");
  CHECK(resonance.getMass() == doctest::Approx(1.6726219e-27));
  CHECK(resonance.getCharge() == 1);
  CHECK(resonance.getWidth() == doctest::Approx(0.1));
}

TEST_CASE("Particle class tests")
{
  lab::Particle::addParticleType("Electron", 9.10938356e-31, -1);
  lab::Particle::addParticleType("Proton", 1.6726219e-27, 1);

  lab::Particle electron("Electron", {1.0, 0.0, 0.0});
  lab::Particle proton("Proton", {0.0, 1.0, 0.0});

  CHECK(electron.getTypeID() == 0);
  CHECK(proton.getTypeID() == 1);

  CHECK(electron.getImpulse()[0] == doctest::Approx(1.0));
  CHECK(proton.getImpulse()[1] == doctest::Approx(1.0));

  // Test sumVecImpulse and normImpulse
  lab::Array3D impulseSum = lab::array3DSum(electron.getImpulse(), proton.getImpulse());
  CHECK(impulseSum[0] == doctest::Approx(1.0));
  CHECK(impulseSum[1] == doctest::Approx(1.0));
  CHECK(impulseSum[2] == doctest::Approx(0.0));

  double norm = lab::euclidianNorm(impulseSum);
  CHECK(norm == doctest::Approx(std::sqrt(2.0)));

  // Test mass and energy
  CHECK(electron.getMass() == doctest::Approx(9.10938356e-31));
  CHECK(proton.getMass() == doctest::Approx(1.6726219e-27));

  double electronEnergy = electron.getEnergy();
  CHECK(electronEnergy
        == doctest::Approx(std::sqrt(9.10938356e-31 * 9.10938356e-31 + 1.0)));

  double protonEnergy = proton.getEnergy();
  CHECK(protonEnergy
        == doctest::Approx(std::sqrt(1.6726219e-27 * 1.6726219e-27 + 1.0)));

  // Test Boost
  lab::Particle particle("Proton", {1.0, 1.0, 1.0});
  particle.boost(0.1, 0.2, 0.3);
  lab::Array3D boostedImpulse = particle.getImpulse();
  CHECK(boostedImpulse[0] != 1.0); // The values should change after the boost
  CHECK(boostedImpulse[1] != 1.0);
  CHECK(boostedImpulse[2] != 1.0);

  // Test Decay2Body
  lab::Particle parent("Proton", {0.0, 0.0, 0.0});
  lab::Particle daughter1("Electron");
  lab::Particle daughter2("Electron");
  int decayResult = parent.decay2Body(daughter1, daughter2);
  CHECK(decayResult == 0); // Check that the decay was successful

  // Verify impulses of daughters are not zero
  lab::Array3D dau1Impulse = daughter1.getImpulse();
  lab::Array3D dau2Impulse = daughter2.getImpulse();
  CHECK(dau1Impulse[0] != 0.0);
  CHECK(dau2Impulse[0] != 0.0);
}
