#include "MOLLEROptSteppingAction.hh"

MOLLEROptSteppingAction::MOLLEROptSteppingAction(MOLLEROptTrackingReadout* TrRO)
{ 
  fSecondary = NULL;
  TrackingReadout = TrRO;
  ErrorVal = G4ThreeVector(1e6,1e6,1e6);
}

void MOLLEROptSteppingAction::UserSteppingAction(const G4Step* theStep)
{ 
  G4Track* theTrack = theStep->GetTrack();

  G4StepPoint *postStep = theStep->GetPostStepPoint();
  G4StepPoint *preStep = theStep->GetPreStepPoint();

  static G4OpBoundaryProcess* boundary=NULL;
  G4OpBoundaryProcessStatus boundaryStatus=Undefined;

  //find the boundary process only once
  if(!boundary){
    G4ProcessManager* pm
      = theStep->GetTrack()->GetDefinition()->GetProcessManager();
    G4int nprocesses = pm->GetProcessListLength();
    G4ProcessVector* pv = pm->GetProcessList();
    G4int i;
    for( i=0;i<nprocesses;i++){
      if((*pv)[i]->GetProcessName()=="OpBoundary"){
        boundary = (G4OpBoundaryProcess*)(*pv)[i];
        break;
      }
    }
  }

  if(theStep->GetTrack()->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition() &&
     theStep->GetTrack()->GetKineticEnergy() < 1.0/MeV){
    theStep->GetTrack()->SetTrackStatus(fStopAndKill);
  }

  if(theStep->GetTrack()->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()){

    if(postStep->GetPhysicalVolume() && preStep->GetPhysicalVolume()){
      if(postStep->GetPhysicalVolume()->GetName() == "Ring5_PMT_Cathode_Physical"
	 || preStep->GetPhysicalVolume()->GetName() == "Ring5_LG_GuideTop_Physical"
	 || preStep->GetPhysicalVolume()->GetName() == "Ring5_PMT_Physical"){
	
	//cout << "Killed in " << postStep->GetPhysicalVolume()->GetName() << endl; 	
	theStep->GetTrack()->SetTrackStatus(fStopAndKill);
      }
    }

    
    boundaryStatus=boundary->GetStatus();
    
    if(postStep->GetStepStatus()==fGeomBoundary){

      // cout << "Status = " << boundaryStatus << endl;
      
      switch(boundaryStatus){
      case Absorption:
	{
	  //cout << "Absorption in " << postStep->GetPhysicalVolume()->GetName() << endl; 
	}
	break;
      case Detection: //Note, this assumes that the volume causing detection
                      //is the photocathode because it is the only one with
                      //non-zero efficiency
        {
	  //cout << "Detection in " << "postStep = " << postStep->GetPhysicalVolume()->GetName() << ", preStep = " << preStep->GetPhysicalVolume()->GetName() << endl;

	  TrackingReadout->IncrementEventCathodeDetection(theStep->GetTrack()->GetTrackID());
	  //theStep->GetTrack()->SetTrackStatus(fStopAndKill);

	  // G4SDManager* SDman = G4SDManager::GetSDMpointer();
          // G4String sdName="/MainDetectorPMTSD";
          // MollerDetMainDetector_PMTSD* pmtSD = (MollerDetMainDetector_PMTSD*)SDman->FindSensitiveDetector(sdName);
          // if(pmtSD)
	  //   {
	  //     if(USERVERBOSELEVEL>0)
	  // 	G4cout << " A photon is detected!" << G4endl;
	  //     //   pmtSD->ProcessHits(theStep,NULL);
	      
	  //     pmtSD->ProcessHits_constStep(theStep,NULL, 1); //last parameter is a flag to indicate that a photon is exactly detected.
	  //   }
          // //trackInformation->AddTrackStatusFlag(hitPMT);


	  
	}
	break;
      case FresnelReflection:
        {
	  //cout << "Fresnel Reflection: " << "postStep = " << postStep->GetPhysicalVolume()->GetName() << ", preStep = " << preStep->GetPhysicalVolume()->GetName() << endl; 	  
	}
	break;
      case LambertianReflection:
        {
	  //cout << "Lambertian Reflection: " << "postStep = " << postStep->GetPhysicalVolume()->GetName() << ", preStep = " << preStep->GetPhysicalVolume()->GetName() << endl; 	  
	}
	break;

	
      case LobeReflection:
        {
	  //cout << "Lobe Reflection: " << "postStep = " << postStep->GetPhysicalVolume()->GetName() << ", preStep = " << preStep->GetPhysicalVolume()->GetName() << endl; 	  
	}
	break;

      case SpikeReflection:
        {
	  //cout << "Spike Reflection: " << "postStep = " << postStep->GetPhysicalVolume()->GetName() << ", preStep = " << preStep->GetPhysicalVolume()->GetName() << endl; 	  
	}
	break;

	 	
 	
 
      }
    }
  }


  
  
  // if(postStep && preStep){

  //   G4VPhysicalVolume *vol = postStep->GetPhysicalVolume();
  //   if(vol){
  //     G4String name = vol->GetName();
  //     G4cout << "Name = " << name << G4endl;
  //     if(postStep->GetPhysicalVolume()->GetName().contains("LG_Physical") &&
  // 	 preStep->GetPhysicalVolume()->GetName().contains("Quartz_Physical")){
	
  // 	DrawHits(true,theTrack);
  //     }
  //   }
  // }
     
  DrawTracks(true, theTrack);

  
  return;
}   

void MOLLEROptSteppingAction::DrawHits(G4bool drawFlag, G4Track *theTrack)
{
  if(drawFlag)
    {
 
      G4ParticleDefinition *particleType = theTrack->GetDefinition();
      
      G4Colour red      ( 255/255.,   0/255.,   0/255.);
      G4Colour blue     (   0/255.,   0/255., 255/255.);
      G4Colour green    (   0/255., 255/255.,   0/255.);
      G4Colour yellow   ( 255/255., 255/255.,   0/255.);
      
      G4Colour white    ( 255/255., 255/255., 255/255.);
      
      G4Colour orange   ( 255/255., 127/255.,   0/255.);
      G4Colour magenta  ( 237/255., 173/255., 255/255.);           
      G4Colour magenta1 ( 104/255.,  49/255.,  94/255.);
      
      G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
      
      if (pVVisManager) {
	const G4SteppingManager* pSM = fpSteppingManager;
	G4Polymarker hitpnt;
        
	G4Colour colour;        

        if( particleType == G4OpticalPhoton::OpticalPhotonDefinition())
          { 
	    colour = magenta;
	  }

        
	G4VisAttributes attribs(colour);
        
	hitpnt.SetVisAttributes(attribs);
	hitpnt.push_back(pSM->GetStep()->GetPostStepPoint()->GetPosition());
        
	pVVisManager -> Draw(hitpnt);
      }
    }
}


void MOLLEROptSteppingAction::DrawTracks(G4bool drawFlag, G4Track *theTrack)
{
  if(drawFlag)
    {
 
      G4ParticleDefinition *particleType = theTrack->GetDefinition();
      
      G4Colour red      ( 255/255.,   0/255.,   0/255.);
      G4Colour blue     (   0/255.,   0/255., 255/255.);
      G4Colour green    (   0/255., 255/255.,   0/255.);
      G4Colour yellow   ( 255/255., 255/255.,   0/255.);
      
      G4Colour white    ( 255/255., 255/255., 255/255.);
      
      G4Colour orange   ( 255/255., 127/255.,   0/255.);
      G4Colour magenta  ( 237/255., 173/255., 255/255.);           
      G4Colour magenta1 ( 104/255.,  49/255.,  94/255.);
      
      G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
      
      if (pVVisManager) {
	const G4SteppingManager* pSM = fpSteppingManager;
	G4Polyline polyline;
        
	G4Colour colour;        

        if( particleType == G4Gamma::GammaDefinition()){
          colour = yellow;
         }
        if( particleType == G4OpticalPhoton::OpticalPhotonDefinition())
          { 
	    colour = blue; }
        if( particleType == G4Electron  ::ElectronDefinition())
          { 
	    colour = red; 
	  }
        if( particleType == G4Positron  ::PositronDefinition())
          { 
	    colour = green; 
	  }


        
	G4VisAttributes attribs(colour);
        
	polyline.SetVisAttributes(attribs);
	polyline.push_back(pSM->GetStep()->GetPreStepPoint()->GetPosition());
	polyline.push_back(pSM->GetStep()->GetPostStepPoint()->GetPosition());
        
	pVVisManager -> Draw(polyline);
      }
    }
}


G4int MOLLEROptSteppingAction::GetTrackVectorStartIndex()
{
  if(!fSecondary) return -1;
  return (*fSecondary).size() - GetTotalNumOfSecondaries();
}
G4int MOLLEROptSteppingAction::GetTrackVectorSize()
{
  if(!fSecondary) return 0;
  return (*fSecondary).size();
}

G4ParticleDefinition *MOLLEROptSteppingAction::GetSecondaryParticleDefinition(G4int idx)
{
  if(!fSecondary || idx >= GetTrackVectorSize() || idx < GetTrackVectorStartIndex()) return NULL; 
  return (*fSecondary)[idx]->GetDefinition();
}
 
G4String MOLLEROptSteppingAction::GetSecondaryParticleName(G4int idx)
{
  if(!fSecondary || idx >= GetTrackVectorSize() || idx < GetTrackVectorStartIndex()) return "undefined"; 
  return (*fSecondary)[idx]->GetDefinition()->GetParticleName();
}

G4double MOLLEROptSteppingAction::GetSecondaryParticleTotalEnergy(G4int idx)
{
  if(!fSecondary || idx >= GetTrackVectorSize() || idx < GetTrackVectorStartIndex()) return -1; 
  return (*fSecondary)[idx]->GetTotalEnergy();
}

G4double MOLLEROptSteppingAction::GetSecondaryParticleKineticEnergy(G4int idx)
{
  if(!fSecondary || idx >= GetTrackVectorSize() || idx < GetTrackVectorStartIndex()) return -1; 
  return (*fSecondary)[idx]->GetKineticEnergy();
}

G4double MOLLEROptSteppingAction::GetSecondaryParticleXOrigin(G4int idx)
{
  if(!fSecondary || idx >= GetTrackVectorSize() || idx < GetTrackVectorStartIndex()) return 1e6; 
  return (*fSecondary)[idx]->GetPosition().x();
}

G4double MOLLEROptSteppingAction::GetSecondaryParticleYOrigin(G4int idx)
{
  if(!fSecondary || idx >= GetTrackVectorSize() || idx < GetTrackVectorStartIndex()) return 1e6; 
  return (*fSecondary)[idx]->GetPosition().y();
}

G4double MOLLEROptSteppingAction::GetSecondaryParticleZOrigin(G4int idx)
{
  if(!fSecondary || idx >= GetTrackVectorSize() || idx < GetTrackVectorStartIndex()) return 1e6; 
  return (*fSecondary)[idx]->GetPosition().z();
}

G4ThreeVector MOLLEROptSteppingAction::GetSecondaryParticleOrigin(G4int idx)
{
  if(!fSecondary || idx >= GetTrackVectorSize() || idx < GetTrackVectorStartIndex()) {return ErrorVal;} 
  return (*fSecondary)[idx]->GetPosition();
}
      
G4ThreeVector MOLLEROptSteppingAction::GetSecondaryParticleMomentum(G4int idx)
{
  if(!fSecondary || idx >= GetTrackVectorSize() || idx < GetTrackVectorStartIndex()) {return ErrorVal;}  
  return (*fSecondary)[idx]->GetMomentumDirection();
}

G4String MOLLEROptSteppingAction::GetSecondaryCreatorProcessName(G4int idx)
{
  if(!fSecondary || idx >= GetTrackVectorSize() || idx < GetTrackVectorStartIndex()) return "undefined"; 
  return (*fSecondary)[idx]->GetCreatorProcess()->GetProcessName();
}

