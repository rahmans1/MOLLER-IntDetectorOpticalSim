#ifndef MOLLEROptPhysicsListMessenger_h
#define MOLLEROptPhysicsListMessenger_h 1

#include "cpp_include.h"
#include "Root_include.h"
#include "Geant4_include.hh" 

#include "MOLLEROptPhysicsList.hh"

class MOLLEROptPhysicsList;

class MOLLEROptPhysicsListMessenger: public G4UImessenger
{
  public:  
    MOLLEROptPhysicsListMessenger(MOLLEROptPhysicsList* );
   ~MOLLEROptPhysicsListMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:  
    MOLLEROptPhysicsList*     pPhysicsList;
    
    G4UIdirectory*        Dir;
    G4UIcmdWithAnInteger* verboseCmd;
};

#endif
