#ifndef MOLLEROptPMTSD_h
#define MOLLEROptPMTSD_h 

#include "cpp_include.h"
#include "Geant4_include.hh"

#include "MOLLEROptPMTHit.hh"
#include "MOLLEROptTrackingReadout.hh"

class G4Step;
class G4HCofThisEvent;

class MOLLEROptPMTSD : public G4VSensitiveDetector
{

public:
  MOLLEROptPMTSD(G4String name, MOLLEROptTrackingReadout* TrRO);
  virtual ~MOLLEROptPMTSD();

  virtual void Initialize(G4HCofThisEvent* HCE);
  virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* ROhist);
  virtual void EndOfEvent(G4HCofThisEvent* HCE);

private:

  MOLLEROptTrackingReadout* TrackingReadout;
  
  MOLLEROptPMTHitsCollection*  HitsCollection;
  G4String theCollectionName;
  G4int theCollectionID;
  const std::vector<const G4Track*>* Secondaries;

};
#endif

