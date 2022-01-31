#include "MOLLEROptPrimaryGeneratorActionMessenger.hh"

MOLLEROptPrimaryGeneratorActionMessenger::MOLLEROptPrimaryGeneratorActionMessenger(MOLLEROptPrimaryGeneratorAction* pPhys)
  :pPrimaryGeneratorAction(pPhys)
{
  Dir = new G4UIdirectory("/Generator/");
  Dir->SetGuidance("UI commands of this example");
 
  verboseCmd = new G4UIcmdWithAnInteger("/Generator/verbose",this);  
  verboseCmd->SetGuidance("set verbose for primary generator");
  verboseCmd->SetParameterName("verbose",true);
  verboseCmd->SetDefaultValue(1);
  verboseCmd->SetRange("verbose>=0");
  verboseCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  resetCmd = new G4UIcmdWithoutParameter("/Generator/ResetNtupleEventCounter",this);  
  resetCmd->SetGuidance("resets Ntuple Event Counter");
  resetCmd->AvailableForStates(G4State_Idle);  

  InitEventCounterCmd = new G4UIcmdWithAnInteger("/Generator/StartingEvent",this);
  InitEventCounterCmd->SetGuidance("set starting event count for primary generator");
  InitEventCounterCmd->SetParameterName("StartingEvent",true);
  InitEventCounterCmd->SetDefaultValue(1);
  InitEventCounterCmd->SetRange("StartingEvent>=0");
  InitEventCounterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  EventHitRegionCmd = new G4UIcmdWithAnInteger("/Generator/EventHitRegion",this);
  EventHitRegionCmd->SetGuidance("Set region in which event hits.");
  EventHitRegionCmd->SetParameterName("EventHitRegion",true);
  EventHitRegionCmd->SetDefaultValue(1);
  EventHitRegionCmd->SetRange("EventHitRegion>=1");
  EventHitRegionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

MOLLEROptPrimaryGeneratorActionMessenger::~MOLLEROptPrimaryGeneratorActionMessenger()
{
  delete verboseCmd;
  delete resetCmd;
  delete Dir;
}

void MOLLEROptPrimaryGeneratorActionMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{       
  if( command == resetCmd )
    {pPrimaryGeneratorAction->ResetNtupleEventCounter(); }

  if( command == InitEventCounterCmd )
    { pPrimaryGeneratorAction->SetNtupleEventCounter(InitEventCounterCmd->GetNewIntValue(newValue)); }

  if( command == EventHitRegionCmd )
    { pPrimaryGeneratorAction->SetEventHitRgion(EventHitRegionCmd->GetNewIntValue(newValue)); }
}
