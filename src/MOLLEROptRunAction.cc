#include "Randomize.hh"
#include "MOLLEROptRunAction.hh"

MOLLEROptRunAction::MOLLEROptRunAction(MOLLEROptAnalysis* AN, MOLLEROptTrackingReadout *TrRO)
  :runID(0), analysis(AN), TrackingReadout(TrRO)
{
  MyRunID = 0;
  ROOTFileFlag = 1;
  RunActionMessenger  = new MOLLEROptRunActionMessenger(this);
}

MOLLEROptRunAction::~MOLLEROptRunAction()
{
  delete RunActionMessenger;
}

void MOLLEROptRunAction::BeginOfRunAction(const G4Run* aRun)
{
  // userInfo->SetMyRunID(MyRunID);
  // userInfo->SetROOTFileFlag(ROOTFileFlag);
  analysis->SetROOTFileFlag(ROOTFileFlag);

  long seeds[2];
  time_t systime = time(NULL);
  seeds[0] = (long) systime;
  seeds[1] = (long) (systime*G4UniformRand());
  CLHEP::HepRandom::setTheSeeds(seeds);
  CLHEP::HepRandom::showEngineStatus();

  TString InfoFile;
  InfoFile.Form("MOLLEROpt_%04d.rndm",MyRunID);
  CLHEP::HepRandom::saveEngineStatus(InfoFile); 
  
  if (G4VVisManager::GetConcreteInstance())
    {
      G4UImanager* UI = G4UImanager::GetUIpointer();
      UI->ApplyCommand("/vis/scene/notifyHandlers");
    } 
  
  analysis->BeginOfRun(MyRunID,TrackingReadout);

}

void MOLLEROptRunAction::EndOfRunAction(const G4Run* aRun)
{ 

  // analysis->SetTotalNumberOfPrimaries((Int_t)userInfo->GetPrimaryEventNumber());
  
  analysis->EndOfRun();
  TrackingReadout->Initialize();

  CLHEP::HepRandom::showEngineStatus();
  
  if (G4VVisManager::GetConcreteInstance())
    {
      G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
    }
}

G4int MOLLEROptRunAction::getRunID()
{
  return runID;
}

