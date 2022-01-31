#ifndef MOLLEROptDetectorMessenger_h
#define MOLLEROptDetectorMessenger_h 1

#include "cpp_include.h"
#include "Root_include.h"
#include "Geant4_include.hh" 

#include "MOLLEROptDetector.hh"

class MOLLEROptDetector;

class MOLLEROptDetectorMessenger: public G4UImessenger
{
  public:
    MOLLEROptDetectorMessenger(MOLLEROptDetector*);
   ~MOLLEROptDetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
private:
  MOLLEROptDetector*         Det;
  
  G4UIdirectory*             Dir;

  G4UIcmdWithAString*        DetMatCmd;

  G4UIcmdWithADoubleAndUnit* DetXPositionCmd;
  G4UIcmdWithADoubleAndUnit* DetYPositionCmd;
  G4UIcmdWithADoubleAndUnit* DetZPositionCmd;


};

#endif
