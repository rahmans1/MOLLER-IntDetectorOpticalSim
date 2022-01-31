#include "MOLLEROptMessenger.hh"

MOLLEROptMessenger::MOLLEROptMessenger(MOLLEROptConstruction* theConstruction)
  :Construction(theConstruction)
{ 

  MOLLEROptDir = NULL;

  MOLLEROptDir = new G4UIdirectory("/MOLLEROpt/");
  MOLLEROptDir->SetGuidance("Set general simulation parameters.");
  
  RadiationDamageCmd = new G4UIcmdWithAnInteger("/MOLLEROpt/QuartzRadDamage",this);
  RadiationDamageCmd->SetGuidance("Update optical radiation damange level on quartz.");
  RadiationDamageCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  RadiationDamageCmd->SetGuidance("The corresponding absorption coefficients are read from a file.");
  RadiationDamageCmd->SetParameterName("Size",true);                   
  RadiationDamageCmd->AvailableForStates(G4State_Idle);

  UpdateGeometryCmd = new G4UIcmdWithoutParameter("/MOLLEROpt/UpdateGeometry",this);
  UpdateGeometryCmd->SetGuidance("Update the geometry");
  UpdateGeometryCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

MOLLEROptMessenger::~MOLLEROptMessenger()
{
  if (RadiationDamageCmd)   delete RadiationDamageCmd;
  if (MOLLEROptDir)         delete MOLLEROptDir;
}

void MOLLEROptMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  //G4cout << newValue << endl;

  if( command == RadiationDamageCmd )
   { 
     Construction->SetRadDamageLevel(RadiationDamageCmd->GetNewIntValue(newValue)); 
   }
  if( command == UpdateGeometryCmd ){ Construction->UpdateGeometry();}


}
