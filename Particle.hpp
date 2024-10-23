#include <string>
#include <vector>

class Particle : public ResonanceType {
    std::array <double, 3> impulse;  
    int index;
    static std::vector<ResonanceType*> typeTable;
    static int findParticle (const std::string &name);
    static void addParticleType(int name,int mass,int charge,double width)
    Particle(const std::string &name, std::array <double, 3> impulse);

    auto getNameArray(int index);
    auto getMassArray(int index);
    auto getChargeArray(int index);
    auto getWidthArray(int index);
    auto getImpulse(int index);

    static std::vector<ParticleType> typeTable;
    static std::map<std::string, int> indexTable;
  
  public:
  Particle(const std::string& name, std::array<double, 3> impulse = {0, 0, 0});
  static void addParticleType(int name, int mass, int charge, double width = 0);
  Particle(const std::string& name, std::array<double, 3> impulse = {0., 0., 0.});

  static void addParticleType(const std::string& name, ParticleType particleType);     
  static void printTypeTable();
  
  int getIndex() const;
  void setIndex(int);
  void setIndex(const std::string&);
}
