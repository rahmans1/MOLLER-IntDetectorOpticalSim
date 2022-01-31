#ifndef MOLLEROptPMTHit_h
#define MOLLEROptPMTHit_h 

//Note: A hit is registered for every step along a track, within a volume

#include "cpp_include.h"
#include "Root_include.h"
#include "Geant4_include.hh"

class MOLLEROptPMTHit : public G4VHit
{
public:

  MOLLEROptPMTHit();
  virtual ~MOLLEROptPMTHit();
  
private:
    
  G4int TrackID;
  G4double PhotonEnergy;
  G4double HitX;
  G4double HitY;
  G4int Particle; 

public:

  void StoreTrackID(G4int id) { TrackID = id;};
  G4double GetTrackID() { return TrackID;};
  void SetParticleType(G4int type) {Particle = type;};
  G4int GetParticleType() {return Particle;};
  
  void AddPhotonHit(G4ThreeVector pos, G4double eng);
  G4double GetPhotonEnergy(){return PhotonEnergy;};
  G4double GetHitLocationX(){return HitX;};
  G4double GetHitLocationY(){return HitY;};

  // G4int GetPhotonVectorLength() {return PhotonEnergy.size();};
  // G4double GetPhotonEnergyAtIndex(G4int n) {if(n >= 0 && n < PhotonEnergy.size()) return PhotonEnergy[n]; return 0;}
  // G4double GetPhotonHitXAtIndex(G4int n) {if(n >= 0 && n < HitX.size()) return HitX[n]; return 20000000;}
  // G4double GetPhotonHitYAtIndex(G4int n) {if(n >= 0 && n < HitY.size()) return HitY[n]; return 20000000;}
  
};

typedef G4THitsCollection<MOLLEROptPMTHit> MOLLEROptPMTHitsCollection;


#endif
