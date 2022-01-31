#ifndef MOLLEROptMessenger_h
#define MOLLEROptMessenger_h 1

#include "cpp_include.h"
#include "Geant4_include.hh" 

#include "MOLLEROptConstruction.hh"
#include "MOLLEROptMaterial.hh"

class MOLLEROptConstruction;

class MOLLEROptMessenger: public G4UImessenger
{
  public:
    MOLLEROptMessenger(MOLLEROptConstruction*);
   ~MOLLEROptMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
private:
  MOLLEROptConstruction* Construction;
  
  G4UIdirectory*             MOLLEROptDir;
  G4UIcmdWithAnInteger*      RadiationDamageCmd;
  G4UIcmdWithoutParameter*   UpdateGeometryCmd;
};

#endif
