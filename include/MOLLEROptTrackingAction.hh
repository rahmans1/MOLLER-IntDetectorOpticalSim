#ifndef MOLLEROptTrackingAction_h
#define MOLLEROptTrackingAction_h 1

#include "cpp_include.h"
#include "Root_include.h"
#include "Geant4_include.hh" 
#include "G4RandomDirection.hh"
#include "MOLLEROptTrackingReadout.hh"

class MOLLEROptTrackingAction : public G4UserTrackingAction
{

public:

   MOLLEROptTrackingAction(MOLLEROptTrackingReadout*);
  ~MOLLEROptTrackingAction();

  void  PreUserTrackingAction(const G4Track* aTrack);
  void PostUserTrackingAction(const G4Track* aTrack);

private:
  
  MOLLEROptTrackingReadout*  TrackingReadout;

};

#endif
