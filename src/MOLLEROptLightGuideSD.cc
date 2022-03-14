#include "MOLLEROptLightGuideSD.hh"

MOLLEROptLightGuideSD::MOLLEROptLightGuideSD(G4String name, MOLLEROptTrackingReadout* TrRO)
  :G4VSensitiveDetector(name)
{
  TrackingReadout = TrRO;
  theCollectionName = G4String("LightGuideHitCollection");
  collectionName.insert(theCollectionName); 
  theCollectionID = -1;
}

MOLLEROptLightGuideSD::~MOLLEROptLightGuideSD()
{

}

void MOLLEROptLightGuideSD::Initialize(G4HCofThisEvent* HCE)
{
  HitsCollection = new MOLLEROptLightGuideHitsCollection(SensitiveDetectorName,collectionName[0]);
  theCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); 
  
  HCE->AddHitsCollection(theCollectionID , HitsCollection);
}

G4bool MOLLEROptLightGuideSD::ProcessHits(G4Step* aStep, G4TouchableHistory* theTouchable)
{
  G4String procName = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();   
  G4StepStatus status = aStep->GetPostStepPoint()->GetStepStatus();   

  G4StepPoint *preStep = aStep->GetPreStepPoint();
  G4StepPoint *postStep = aStep->GetPostStepPoint();
  
  G4ProcessManager* OpManager = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
  G4int MAXofPostStepLoops = OpManager->GetPostStepProcessVector()->entries();
  G4ProcessVector* postStepDoItVector = OpManager->GetPostStepProcessVector(typeDoIt);
  G4VProcess* currentProcess;
  G4OpBoundaryProcessStatus theStatus = Undefined;
  // MOLLEROptLightGuideHit* aHit = new MOLLEROptLightGuideHit();
  G4double incidentAngle = -180;

  if(aStep->GetTrack()->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){

    // if(procName.compare("Transportation") == 0){

    

      if(status == fGeomBoundary){
	
	// if(aHit){

	  for (G4int i=0; i<MAXofPostStepLoops; ++i) {
	    currentProcess = (*postStepDoItVector)[i];
	    G4OpBoundaryProcess* opProc = dynamic_cast<G4OpBoundaryProcess*>(currentProcess);
	    if (opProc) {
	      theStatus = opProc->GetStatus();
	      
	      if(theStatus == FresnelReflection ||
		 theStatus == TotalInternalReflection ||
		 theStatus == LambertianReflection ||
		 theStatus == LobeReflection ||
		 theStatus == SpikeReflection){

		G4ThreeVector imom = preStep->GetMomentumDirection();
		G4ThreeVector fmom = -1.0*postStep->GetMomentumDirection();
		//G4double tmpA = TMath::ACos(imom.dot(fmom));
		
		incidentAngle = 90.0 - 180*0.5*(1 - TMath::ACos(imom.dot(fmom))/TMath::Pi());

		// aHit->StoreStepLength(aStep->GetStepLength());
		// aHit->StoreTrackID(aStep->GetTrack()->GetTrackID());
		// aHit->SetParticleType(10); //photon
		// aHit->StorePhotonEnergy(aStep->GetTrack()->GetKineticEnergy());
		// aHit->StoreIncidentPhotonAngle(incidentAngle);
		// aHit->StoreReflectionProcess(theStatus);
		// HitsCollection->insert(aHit);

	      }	
	    }
	  }
	  //}
      }
      TrackingReadout->AddTrackData(aStep->GetTrack()->GetTrackID(),myPhoton,
				    aStep->GetStepLength(),-1,0,myLightGuide,0,
				    aStep->GetTrack()->GetKineticEnergy(),
				    1239.842/(aStep->GetTrack()->GetKineticEnergy()/eV),
				    incidentAngle);
      
      
      //}
  }  
  
  return true;
}

void MOLLEROptLightGuideSD::EndOfEvent(G4HCofThisEvent* )
{

}

      
  // G4TouchableHandle touchable = preStep->GetTouchableHandle();
  // G4ThreeVector worldpos = preStep->GetPosition();
  // G4ThreeVector localpos = touchable->GetHistory()->GetTopTransform().TransformPoint(worldpos);
  // G4VSolid * sector = touchable->GetSolid();
  // G4ThreeVector norm = -sector->SurfaceNormal(localpos);
  // G4ThreeVector photonDir = preStep->GetMomentum()/preStep->GetMomentum().mag();
  // const G4RotationMatrix *rot = touchable->GetRotation();
  // G4ThreeVector photonDirLoc = *rot*photonDir;
  // //G4double incidentAngle = acos(norm.dot(photonDirLoc))*180.0/TMath::Pi() - 90;
  // G4double incidentAngle = norm.dot(photonDirLoc);
  
  // G4StepPoint *preStep = aStep->GetPreStepPoint();
  // G4ThreeVector stepPos = preStep->GetPosition();
  // G4ThreeVector photonDir = preStep->GetMomentum()/preStep->GetMomentum().mag();
  // const G4VTouchable * touchable = preStep->GetTouchable();
  // const G4RotationMatrix * rot = touchable->GetRotation();
  // G4RotationMatrix rotInv = rot->inverse();
  // G4ThreeVector transl = touchable->GetTranslation();
  // G4VSolid * sector = touchable->GetSolid();
  // //G4String name = touchable->GetVolume()->GetName();
  // G4ThreeVector posLoc = *rot * (stepPos - transl);
  // G4ThreeVector norm = sector->SurfaceNormal(posLoc);
  

