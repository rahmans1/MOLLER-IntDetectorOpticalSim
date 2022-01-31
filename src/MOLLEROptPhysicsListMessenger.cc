#include "MOLLEROptPhysicsListMessenger.hh"

MOLLEROptPhysicsListMessenger::MOLLEROptPhysicsListMessenger(MOLLEROptPhysicsList* pPhys)
:pPhysicsList(pPhys)
{
  Dir = new G4UIdirectory("/Physics/");
  Dir->SetGuidance("UI commands of this example");
 
  verboseCmd = new G4UIcmdWithAnInteger("/Physics/verbose",this);  
  verboseCmd->SetGuidance("set verbose for physics processes");
  verboseCmd->SetParameterName("verbose",true);
  verboseCmd->SetDefaultValue(1);
  verboseCmd->SetRange("verbose>=0");
  verboseCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
}

MOLLEROptPhysicsListMessenger::~MOLLEROptPhysicsListMessenger()
{
  delete verboseCmd;
  delete Dir;
}

void MOLLEROptPhysicsListMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{       
  if( command == verboseCmd )
   { pPhysicsList->SetVerbose(verboseCmd->GetNewIntValue(newValue));}
   
}
