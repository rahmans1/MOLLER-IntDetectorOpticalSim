#ifndef MOLLEROptLightGuideHit_h
#define MOLLEROptLightGuideHit_h 

//Note: A hit is registered for every step along a track, within a volume

#include "cpp_include.h"
#include "Root_include.h"
#include "Geant4_include.hh"

class MOLLEROptLightGuideHit : public G4VHit
{
public:

  MOLLEROptLightGuideHit();
  virtual ~MOLLEROptLightGuideHit();
  
private:
  
  G4double StepLength;               //Step length for this hit
  G4int TrackID;
  std::vector<G4double> SecPhotonEnergy;
   
  G4int Particle;                  //Type of particle that produced this hit; parameterized according to the definitions in TrackingInformation.hh 

  G4ThreeVector TrackVertexMomentum;
  G4ThreeVector TrackVertexPosition;
  G4double      PhotonEnergy;
  G4double      IncidentPhotonAngle;
  G4OpBoundaryProcessStatus ReflectionProcess;

public:
  
  void StorePhotonEnergy(G4double eng){PhotonEnergy = eng;};
  G4double GetPhotonEnergy(){return PhotonEnergy;};
  
  void StoreStepLength(G4double dx) { StepLength = dx;};
  G4double GetStepLength() { return StepLength;};

  void StoreTrackID(G4int id) { TrackID = id;};
  G4double GetTrackID() { return TrackID;};

  void  SetParticleType(G4int type) { Particle = type; }
  G4int GetParticleType(){ return Particle; }

  void StoreTrackVertexMomentum(G4ThreeVector vmom) {TrackVertexMomentum = vmom;};
  void StoreTrackVertexPosition(G4ThreeVector vpos) {TrackVertexPosition = vpos;};
  void StoreIncidentPhotonAngle(G4double angle) {IncidentPhotonAngle = angle;};
  void StoreReflectionProcess(G4OpBoundaryProcessStatus proc) {ReflectionProcess = proc;};

  G4ThreeVector GetTrackVertexMomentum(){return TrackVertexMomentum;};
  G4ThreeVector GetTrackVertexPosition(){return TrackVertexPosition;};
  G4double GetIncidentPhotonAngle() {return IncidentPhotonAngle;};
  G4OpBoundaryProcessStatus GetReflectionProcess() {return ReflectionProcess;};
  
  
  void AddSecondaryPhoton(G4double eng) {SecPhotonEnergy.push_back(eng);};
  G4int GetSecondaryPhotonVectorLength() {return SecPhotonEnergy.size();};
  G4double GetSecondaryPhotonEnergyAtIndex(G4int n) {if(n >= 0 && n < SecPhotonEnergy.size()) return SecPhotonEnergy[n]; return 0;}
  
};

typedef G4THitsCollection<MOLLEROptLightGuideHit> MOLLEROptLightGuideHitsCollection;


#endif
