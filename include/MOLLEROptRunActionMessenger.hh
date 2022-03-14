#ifndef MOLLEROptRunActionMessenger_h
#define MOLLEROptRunActionMessenger_h 1

#include "cpp_include.h"
#include "Geant4_include.hh" 

#include "MOLLEROptRunAction.hh"

class MOLLEROptRunAction;

class MOLLEROptRunActionMessenger: public G4UImessenger
{
public:
    MOLLEROptRunActionMessenger(MOLLEROptRunAction*);
  ~MOLLEROptRunActionMessenger();
  
  void SetNewValue(G4UIcommand*, G4String);
  
private:
  MOLLEROptRunAction*          RunAction;
  G4UIdirectory*             Dir;
  
  G4UIcmdWithAnInteger*      MyRunIDCmd;
  G4UIcmdWithAString*        MyRunFileNameCmd;
  G4UIcmdWithAString*        MyRunDirNameCmd;
  G4UIcmdWithAnInteger*      ROOTFileFlagCmd;


};

#endif
