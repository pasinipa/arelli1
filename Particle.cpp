#include "particle.hpp"

//#include <chrono>
//#include <random>
//#include <cmath>

Particle::Particle(const std::string& name, std::array<double, 3> impulse = {0, 0, 0}) : impulse{impulse}
{
  if (indexTable.contains(name))
    index{indexTable[name]};
  // else throw ? e poi stampare a schermo il fatto che c'è stato un problema
}

static void Particle::addParticleType(const std::string& name, const ParticleType& particleType) {
  if (not indexTable.contains(name)){ 
    // push_back or emplace?
    typeTable.push_back(particleType);
    indexTable.insert({name, --typeTable.size()});
  }
  //else non fa niente oppure aggiorna la sua definizione di quella particella?
}

static void Particle::printTypeTable() {
  for (const auto& t : typeTable) {
    t.print();
    std::cout << '\n';
  }
}

void Particle::printParticle(){
  std::cout /*<< "Name:" << Particle::name << '\n'*/
            << "Impulse first component:" << Particle::impulse[0] << '\n'
            << "Impulse second component:" << Particle::impulse[1] << '\n'
            << "Impulse third component:" << Particle::impulse[2] << '\n';
}

/*int Particle::calculateEnergy(){
  energy = std::sqrt(std::pow(Particle::mass,2) 
              + std::pow(Particle::impulse[0],2)
              + std::pow(Particle::impulse[1],2)
              + std::pow(Particle::impulse[2],2))
}*/





/*static auto seed = static_cast<unsigned int>(
    std::chrono::steady_clock::now().time_since_epoch().count());
static std::default_random_engine eng(seed);
static std::uniform_real_distribution<double> angleThetaDistribution(0, 360);
static std::uniform_real_distribution<double> anglePhiDistribution(0, 360);
static std::exponential_distribution<double> impulseModuleDistribution(0, MAX_IMPULSE);
oppure
static std::exponential_distribution<double> impulseComponentDistribution(0, MAX_IMPULSE);

Particle::Particle()
    : angTheta{angleThetaDistribution(eng))}
    , m_velocity{velDistribution(eng), velDistribution(eng)}
    , impulseModule{}   
    oppure 
    ,impulse{impulseComponentDistribution(eng), impulseComponentDistribution(eng), impulseComponentDistribution(eng)}
{
  bisogna o comporre l'impulso se usiamo gli angoli oppure facciamo controlli su possibili valori troppo alti del modulo
}

*/ 


