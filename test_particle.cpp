#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "Particle.hpp"
#include "doctest.h"
#include "resonance_type.hpp"

TEST_CASE("ParticleType class tests")
{
  pt::ParticleType electron("Electron", 9.10938356e-31, -1);

  CHECK(electron.getName() == "Electron");
  CHECK(electron.getMass() == doctest::Approx(9.10938356e-31));
  CHECK(electron.getCharge() == -1);
}

TEST_CASE("ResonanceType class tests")
{
  rt::ResonanceType resonance("Resonance", 1.6726219e-27, 1, 0.1);

  CHECK(resonance.getName() == "Resonance");
  CHECK(resonance.getMass() == doctest::Approx(1.6726219e-27));
  CHECK(resonance.getCharge() == 1);
  CHECK(resonance.getWidth() == doctest::Approx(0.1));
}

TEST_CASE("Particle class tests")
{
  p::Particle::addParticleType("Electron", 9.10938356e-31, -1);
  p::Particle::addParticleType("Proton", 1.6726219e-27, 1);

  p::Particle electron("Electron", {1.0, 0.0, 0.0});
  p::Particle proton("Proton", {0.0, 1.0, 0.0});

  CHECK(electron.getIndex() == 0);
  CHECK(proton.getIndex() == 1);

  CHECK(electron.getImpulse().px_ == doctest::Approx(1.0));
  CHECK(proton.getImpulse().py_ == doctest::Approx(1.0));

  // Test sumVecImpulse and normImpulse
  p::Impulse impulseSum =
      p::sumVecImpulse(electron.getImpulse(), proton.getImpulse());
  CHECK(impulseSum.px_ == doctest::Approx(1.0));
  CHECK(impulseSum.py_ == doctest::Approx(1.0));
  CHECK(impulseSum.pz_ == doctest::Approx(0.0));

  double norm = p::normImpulse(impulseSum);
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
}
