#ifndef MOLLEROptDetSD_h
#define MOLLEROptDetSD_h 

#include "cpp_include.h"
#include "Geant4_include.hh"

#include "MOLLEROptDetHit.hh"
#include "MOLLEROptTrackInformation.hh"

class G4Step;
class G4HCofThisEvent;

class MOLLEROptDetSD : public G4VSensitiveDetector
{

public:
  MOLLEROptDetSD(G4String name, G4int cell);
  virtual ~MOLLEROptDetSD();

  virtual void Initialize(G4HCofThisEvent* HCE);
  virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* ROhist);
  virtual void EndOfEvent(G4HCofThisEvent* HCE);

private:
  
  MOLLEROptDetHitsCollection*  HitsCollection;
  G4String theCollectionName;
  G4int theCollectionID;

};
#endif


