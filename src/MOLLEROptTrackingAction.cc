#include "MOLLEROptTrackingAction.hh"

MOLLEROptTrackingAction::MOLLEROptTrackingAction(MOLLEROptTrackingReadout* TrRO)
  : TrackingReadout(TrRO)
{

}

MOLLEROptTrackingAction::~MOLLEROptTrackingAction()
{

}

void MOLLEROptTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{    
  if( TrackingReadout->TrackExists(aTrack->GetTrackID()) ) return;

  TrackData *nTrack = new TrackData;
  const G4VProcess *proc = aTrack->GetCreatorProcess();
  nTrack->ID = aTrack->GetTrackID();
  nTrack->ParentID = aTrack->GetParentID();

  if(aTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){
    nTrack->Particle = myPhoton;
    // if(aTrack->GetTrackID() != 1){
    //   G4cout << "Creator Process = " << proc->GetProcessName() << G4endl;
    // }
  }
  else if(aTrack->GetDefinition() == G4Electron::ElectronDefinition() &&  aTrack->GetKineticEnergy() > 1.0/MeV){
    nTrack->Particle = myElectron;
    // G4cout << "ID = " << aTrack->GetTrackID() << G4endl;
    // G4cout << "Parent ID = " << aTrack->GetParentID() << G4endl;
    // G4cout << "Track Energy = " << aTrack->GetKineticEnergy()/keV << G4endl;      
    // if(aTrack->GetTrackID() != 1){
    //   G4cout << "Creator Process = " << proc->GetProcessName() << G4endl;
    // }
  }
  else
    nTrack->Particle = 0;
    
  nTrack->Length = 0;
  nTrack->NSteps = 0;
  nTrack->QLength = 0;
  nTrack->QSteps = 0;
  nTrack->LGLength = 0;
  nTrack->LGSteps = 0;
  nTrack->QExitFlag = 0;
  nTrack->PMTHitFlag = 0;
  nTrack->LGHitFlag = 0;
  nTrack->PMTHitX = 6666;
  nTrack->PMTHitY = 6666;
  nTrack->PMTHitZ = 6666;
  nTrack->Detected = 0;
  
  nTrack->InitEnergy = aTrack->GetTotalEnergy();
  nTrack->InitKinEnergy = aTrack->GetKineticEnergy();
  nTrack->InitWavelength = 1293.842/(aTrack->GetKineticEnergy()/eV); 
  nTrack->InitMomDirX = aTrack->GetMomentumDirection().x();
  nTrack->InitMomDirY = aTrack->GetMomentumDirection().y();
  nTrack->InitMomDirZ = aTrack->GetMomentumDirection().z();

  TrackingReadout->AddNewTrack(nTrack);
}

void MOLLEROptTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{

}
