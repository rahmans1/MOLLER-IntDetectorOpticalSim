#ifndef MOLLEROptLightGuideSD_h
#define MOLLEROptLightGuideSD_h 

#include "cpp_include.h"
#include "Geant4_include.hh"

#include "MOLLEROptLightGuideHit.hh"
#include "MOLLEROptTrackingReadout.hh"

class G4Step;
class G4HCofThisEvent;

class MOLLEROptLightGuideSD : public G4VSensitiveDetector
{

public:
  MOLLEROptLightGuideSD(G4String name, MOLLEROptTrackingReadout* TrRO);
  virtual ~MOLLEROptLightGuideSD();

  virtual void Initialize(G4HCofThisEvent* HCE);
  virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* ROhist);
  virtual void EndOfEvent(G4HCofThisEvent* HCE);

private:

  MOLLEROptTrackingReadout* TrackingReadout;
  
  MOLLEROptLightGuideHitsCollection*  HitsCollection;
  G4String theCollectionName;
  G4int theCollectionID;

};
#endif


