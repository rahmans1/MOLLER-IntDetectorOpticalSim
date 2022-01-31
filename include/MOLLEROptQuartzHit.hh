#ifndef MOLLEROptQuartzHit_h
#define MOLLEROptQuartzHit_h 

//Note: A hit is registered for every step along a track, within a volume

#include "cpp_include.h"
#include "Root_include.h"
#include "Geant4_include.hh"

class MOLLEROptQuartzHit : public G4VHit
{
public:

  MOLLEROptQuartzHit();
  virtual ~MOLLEROptQuartzHit();
  
private:
  
  G4double StepLength;               //Step length for this hit
  G4int TrackID;
  std::vector<G4double> SecPhotonEnergy;  
  std::vector<G4int> SecPhotonID;  
 
  G4int Particle;                  //Type of particle that produced this hit; parameterized according to the definitions in TrackingInformation.hh 

  G4ThreeVector TrackVertexMomentum;
  G4ThreeVector TrackVertexPosition;
  G4double      PhotonEnergy;
  G4int         PhotonAtExit;
  
public:
  
  void StorePhotonEnergy(G4double eng){PhotonEnergy = eng;};
  G4double GetPhotonEnergy(){return PhotonEnergy;};
  
  void StoreStepLength(G4double dx) { StepLength = dx;};
  G4double GetStepLength() { return StepLength;};

  void StoreTrackID(G4int id) { TrackID = id;};
  G4double GetTrackID() { return TrackID;};
  
  void  SetParticleType(G4int type) { Particle = type; }
  G4int GetParticleType(){ return Particle; }

  void SetPhotonAtExit(G4int yn){ PhotonAtExit = yn;};
  G4int IsPhotonAtExit(){return PhotonAtExit;};

  void StoreTrackVertexMomentum(G4ThreeVector vmom) {TrackVertexMomentum = vmom;};
  void StoreTrackVertexPosition(G4ThreeVector vpos) {TrackVertexPosition = vpos;};

  G4ThreeVector GetTrackVertexMomentum(){return TrackVertexMomentum;};
  G4ThreeVector GetTrackVertexPosition(){return TrackVertexPosition;};

  void AddSecondaryPhotonEnergy(G4double eng) {SecPhotonEnergy.push_back(eng);};
  void AddSecondaryPhotonID(G4int id) {SecPhotonID.push_back(id);};
  G4int GetSecondaryPhotonVectorLength() {return SecPhotonID.size();};
  G4double GetSecondaryPhotonEnergyAtIndex(G4int n) {if(n >= 0 && n < SecPhotonEnergy.size()) return SecPhotonEnergy[n]; return 0;}
  G4int GetSecondaryPhotonIDAtIndex(G4int n) {if(n >= 0 && n < SecPhotonID.size()) return SecPhotonID[n]; return 0;}
  
};

typedef G4THitsCollection<MOLLEROptQuartzHit> MOLLEROptQuartzHitsCollection;


#endif
