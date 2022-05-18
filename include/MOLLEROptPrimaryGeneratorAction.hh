#ifndef MOLLEROptPrimaryGeneratorAction_h
#define MOLLEROptPrimaryGeneratorAction_h 

#include "cpp_include.h"
#include "Root_include.h"
#include "Geant4_include.hh" 
#include "MOLLEROptConstruction.hh"

#include "MOLLEROptPrimaryGeneratorActionMessenger.hh"

class MOLLEROptPrimaryGeneratorActionMessenger;
class G4ParticleGun;
class G4Event;
 
class MOLLEROptPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
   MOLLEROptPrimaryGeneratorAction(MOLLEROptConstruction*);    
  ~MOLLEROptPrimaryGeneratorAction();
  
public:

  void GeneratePrimaries(G4Event* anEvent);
  void ResetNtupleEventCounter() {EventCounter = 0;};
  void SetNtupleEventCounter(G4int cnt) {EventCounter = cnt;};
  void SetEventHitRegion(G4int reg) {EventRegion = reg; Construction->SetEventHitRegion(reg);};
  void SetEventHitRegionCenterOffsetX(G4double offx) {EventRegionOffsetX = offx;}
  void SetEventHitRegionCenterOffsetY(G4double offy) {EventRegionOffsetY = offy;}    
  
private:

  G4int EventRegion;
  G4int EventCounter;
  G4double EventRegionOffsetX;
  G4double EventRegionOffsetY;
  
  G4ParticleGun* particleGun;

  MOLLEROptConstruction* Construction;
  
  MOLLEROptPrimaryGeneratorActionMessenger* Messenger;

};

#endif
