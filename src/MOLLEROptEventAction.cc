#include "MOLLEROptEventAction.hh"

MOLLEROptEventAction::MOLLEROptEventAction(MOLLEROptAnalysis* AN, MOLLEROptTrackingReadout* TrRO)
{
  analysis    = AN;
  TrackingReadout  = TrRO;

  PMTSecOptPhotonCnt = new TH1D("PMTSecOptPhotonCnt","",800,100,900);
  PMTSecOptPhotonCnt->SetBit(TH1::kIsAverage);

  QuartzSecOptPhotonCnt = new TH1D("QuartzSecOptPhotonCnt","",800,100,900);
  QuartzSecOptPhotonCnt->SetBit(TH1::kIsAverage);
  
  LightGuideSecOptPhotonCnt = new TH1D("LightGuideSecOptPhotonCnt","",800,100,900);
  LightGuideSecOptPhotonCnt->SetBit(TH1::kIsAverage);
  
  oFile.open("TestFile.txt");

  TrackingReadout->Initialize();

}


MOLLEROptEventAction::~MOLLEROptEventAction()
{
  oFile.close();
}


void MOLLEROptEventAction::BeginOfEventAction(const G4Event* evt)
{
 
  G4SDManager * SDman = G4SDManager::GetSDMpointer();

  CollID[0] = SDman->GetCollectionID("Quartz/QuartzHitCollection");
  CollID[1] = SDman->GetCollectionID("LightGuide/LightGuideHitCollection");
  CollID[2] = SDman->GetCollectionID("Cathode/PMTHitCollection");
  
  PMTSecOptPhotonCnt->Reset();
  QuartzSecOptPhotonCnt->Reset();
  LightGuideSecOptPhotonCnt->Reset();

  for(int n = 0; n < ProcessedTrack.size(); n++) delete ProcessedTrack[n];
  ProcessedTrack.clear();
  ProcessedTrack.resize(0);
  TrackingReadout->Initialize();
  // G4cout << "Event ID: " <<  evt->GetEventID() << G4endl;

}

Bool_t MOLLEROptEventAction::TrackHasBeenProcessed(G4int id)
{
  Bool_t check = false;
  for(int n = 0; n < ProcessedTrack.size(); n++)
    if(ProcessedTrack[n]->ID == id) check = true;

  return check;
}

void MOLLEROptEventAction::AddQuartzTrackStep(G4int id, G4int step)
{
  for(int n = 0; n < ProcessedTrack.size(); n++){
    if(ProcessedTrack[n]->ID == id){
      ProcessedTrack[n]->Length += step;
      ProcessedTrack[n]->Steps++;
      return;
    }
  }

  lTrackData *data = new lTrackData;
  data->ID = id;
  data->Length = step;
  data->Steps = 1;
  
  ProcessedTrack.push_back(data);

  return;
}


void MOLLEROptEventAction::EndOfEventAction(const G4Event* evt)
{

  Initialize();
  analysis->MOLLERMainEvent->MOLLERPrimEvent.Initialize(); 

  G4int hitCnt1, hitCnt2, PMThit, qtrackID, lgtrackID, pmttrackID, ctrackID, LGSteps, QSteps, TSteps, secPhCnt; 
  G4double LGTrackLength, QuartzTrackLength, TotalTrackLength;
  G4double PMTPe = 0;
  G4int NumSecPhotons = 0;
  G4int hitflag = 0;
 
  Float_t  optPhEng, wvl, bwdt = QuartzSecOptPhotonCnt->GetBinWidth(2);

  // G4HCofThisEvent * HCE = evt->GetHCofThisEvent();

  ctrackID = -1;

  G4int NumTracks = TrackingReadout->GetNumTracks();
  // G4cout << "Number of electron tracks = " << TrackingReadout->GetNumElectronTracks() << G4endl;
  // G4cout << "Number of photon tracks = " << TrackingReadout->GetNumPhotonTracks() << G4endl;

  TrackData *track;
  for(int t = 0; t < NumTracks; t++){

    PMThit = 0;
    
    analysis->MOLLERMainEvent->MOLLERDetectorEvent.Initialize();  

    track  = TrackingReadout->GetTrackData(t);
    if(track){

      analysis->MOLLERMainEvent->MOLLERDetectorEvent.SetEventID(evt->GetEventID());
      analysis->MOLLERMainEvent->MOLLERDetectorEvent.SetTrackParentID(track->ParentID);      
      analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddTrackInitMomDirection(track->InitMomDirX,track->InitMomDirY,
									      track->InitMomDirZ);      
      
      if(track->Particle == myElectron){
	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddElectronTrackID(track->ID);
	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddPhotonTrackID(0);

	if(track->QuartzHitFlag){
	  analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzTrackHit(1);
	  analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzHitPositionX((Float_t)track->QuartzHitX/cm);
	  analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzHitPositionY((Float_t)track->QuartzHitY/cm);
	}	
	// for(int p = 0; p < track->SecPhotonAngle.size(); p++){
	// }
	for(int p = 0; p < track->StepNChPhotons.size(); p++){
	  // analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzTrackSecPhotonAngle(track->SecPhotonAngle[p]);
	  analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzStepNPhotons(track->StepNChPhotons[p]);
	  analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzElectronStepLength(track->StepLength[p]/cm);
	}
	for(int p = 0; p < track->SecPhotonAngle.size(); p++)
	  analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzTrackSecPhotonAngle(track->SecPhotonAngle[p]);
      }
      if(track->Particle == myPhoton){
	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddPhotonTrackID(track->ID);	
	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddElectronTrackID(0);
	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzTrackData(track->QLength/cm, track->QSteps);
	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzPhotonEnergy(track->InitKinEnergy/eV);
	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzPhotonAtExitFlag(track->QExitFlag);
	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideTrackData(track->LGLength/cm, track->LGSteps);
	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddPMTTrackHit(track->PMTHitFlag);
	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideTrackHit(track->LGHitFlag);
	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddPMTPhotonEnergy(track->InitKinEnergy/eV);
	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuidePhotonEnergy(track->InitKinEnergy/eV);
	QuartzSecOptPhotonCnt->Fill(track->InitWavelength,1.0/(bwdt));
	if(track->QExitFlag)
	  LightGuideSecOptPhotonCnt->Fill(track->InitWavelength,1.0/(bwdt));
	if(track->PMTHitFlag){
	  PMTSecOptPhotonCnt->Fill(track->InitWavelength,1.0/(bwdt));
	  analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddPMTHitPositionX((Float_t)track->PMTHitX/cm);
	  analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddPMTHitPositionY((Float_t)track->PMTHitY/cm);
	  analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddPMTWindowReflectionAngle((Float_t)track->PMTWinRefl);
	  analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddPMTPhotonEnergy(track->InitKinEnergy/eV);
	  optPhEng = track->InitKinEnergy/eV;
	  OptParam* op = TrackingReadout->GetOpticalParameters();
	  for(int n = 0; n < op->npar-1; n++)
	    if(optPhEng >= op->EPhoton[n]/eV && optPhEng < op->EPhoton[n+1]/eV){
	      PMTPe += gRandom->PoissonD(op->QEff[n]);
	    }
	  PMThit++;
	}
	  
	for(int s = 0; s < track->NSteps; s++){
	  if(track->StepVolume[s] == myQuartz){
	    analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzStepLength(track->StepLength[s]/cm);
	    analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzIncidentPhotonAngle(track->StepAngle[s]);
	  }
	  if(track->StepVolume[s] == myLightGuide){
	    analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideStepLength(track->StepLength[s]/cm);
	    analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideIncidentPhotonAngle(track->StepAngle[s]);
	  }
	}
	
      }
    }
    analysis->FillRootNtuple();
  }

  analysis->AddPhotoElectronEvent(PMTPe);
  analysis->AddCathodeDetectionEvent(TrackingReadout->GetCathodeDetections());
  
  for(int n = 1; n <= QuartzSecOptPhotonCnt->GetNbinsX(); n++)
    analysis->AddToAverageQuartzOptPhotonDist(QuartzSecOptPhotonCnt->GetBinCenter(n),QuartzSecOptPhotonCnt->GetBinContent(n));
  for(int n = 1; n <= LightGuideSecOptPhotonCnt->GetNbinsX(); n++)
    analysis->AddToAverageLightGuideOptPhotonDist(LightGuideSecOptPhotonCnt->GetBinCenter(n),LightGuideSecOptPhotonCnt->GetBinContent(n));
  for(int n = 1; n <= PMTSecOptPhotonCnt->GetNbinsX(); n++)
    analysis->AddToAveragePMTOptPhotonDist(PMTSecOptPhotonCnt->GetBinCenter(n),PMTSecOptPhotonCnt->GetBinContent(n));
  

  // // 	    //   LGTrackLength = 0;
  // // 	    //   LGSteps = 0;
  // // 	    //   for(int j = 0; j < hitCnt2; j++){ 

  // // 	    // 	LightGuideHit = (*LightGuideHC)[j];
  // // 	    // 	if(LightGuideHit){
		  
  // // 	    // 	  if(LightGuideHit->GetParticleType() == 10){ //this is a photon
		    
  // // 	    // 	    if(LightGuideHit->GetTrackID() == ctrackID){
  // // 	    // 	      LGTrackLength += LightGuideHit->GetStepLength(); 
  // // 	    // 	      LGSteps++;  //total number of steps the photon took to get to the light guide exit
  // // 	    // 	      analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuidePhotonEnergy((Float_t)LightGuideHit->GetPhotonEnergy()/eV);
  // // 	    // 	      analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideStepLength((Float_t)LightGuideHit->GetStepLength()/cm);
  // // 	    // 	      analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideIncidentPhotonAngle((Float_t)LightGuideHit->GetIncidentPhotonAngle());
  // // 	    // 	      analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideReflectionProcess((Int_t)LightGuideHit->GetReflectionProcess());
  // // 	    // 	      wvl = 1239.842/(LightGuideHit->GetPhotonEnergy()/eV);
  // // 	    // 	      LightGuideSecOptPhotonCnt->Fill(wvl,1.0/(bwdt));		      
  // // 	    // 	    }
  // // 	    // 	  }
  // // 	    // 	}
  // // 	    //   }
  // // 	    //   if(LGSteps)
  // // 	    // 	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideTrackHit(1);
  // // 	    //   else
  // // 	    // 	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideTrackHit(0);
  // // 	    //   analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideTrackData((Float_t)LGTrackLength/cm, (Int_t)LGSteps);

  
  //=======================================================================
  // Save the Ntuple periodically so we have some data in case of a crash
  
  G4int eventNumber = evt->GetEventID();
  
  if (eventNumber%100 == 1) analysis->AutoSaveRootNtuple();
  //=======================================================================
  
  
} // end of  MOLLEROptEventAction::EndOfEventAction()

void MOLLEROptEventAction::Initialize()
{	  
  
  primaryEventNumber = 0;
  
}



	
    // if(PMTHC){
      
    //   hitCnt1 = PMTHC->entries();
      
    //   for(int i = 0; i < hitCnt1; i++){ //Note that each step inside a cell is a hit here
		
    // 	PMTHit = (*PMTHC)[i];
    // 	if(PMTHit){
	  
    // 	  trackID = PMTHit->GetTrackID();
	  
    // 	  if(ctrackID != trackID){
	    
    // 	    analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddPMTHitTrackID(trackID);
    // 	    analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddPMTPhotonEnergy((Float_t)PMTHit->GetPhotonEnergy()/eV);
    // 	    analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddPMTHitPositionX((Float_t)PMTHit->GetHitLocationX()/cm);
    // 	    analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddPMTHitPositionY((Float_t)PMTHit->GetHitLocationY()/cm);
    // 	    ctrackID = trackID;
	    
    // 	    wvl = 1239.842/(PMTHit->GetPhotonEnergy()/eV);
    // 	    PMTSecOptPhotonCnt->Fill(wvl,1.0/(bwdt));
    // 	  }
    // 	}
    //   }
    //   for(int n = 1; n <= PMTSecOptPhotonCnt->GetNbinsX(); n++)
    // 	analysis->AddToAveragePMTOptPhotonDist(PMTSecOptPhotonCnt->GetBinCenter(n),PMTSecOptPhotonCnt->GetBinContent(n));	  
      
    // }

    // QuartzTrackLength = 0;
    // QSteps = 0;
    
    // LGTrackLength = 0;
    // LGSteps = 0;

    // TotalTrackLength = 0;	
    // TSteps = 0;
    
    // ctrackID = -1;
    // if(LightGuideHC){

    //   hitCnt2 = LightGuideHC->entries();

    //   if(hitCnt2){
	
    // 	for(int j = 0; j < hitCnt2; j++){ //Note that each step inside a volume is a hit here    
    // 	  LightGuideHit = (*LightGuideHC)[j];
    // 	  if(LightGuideHit){

    // 	    trackID = LightGuideHit->GetTrackID();

    // 	    if(ctrackID != trackID){
	      
    // 	      if(LightGuideHit->GetParticleType() == 10){ //photon		  
		      
    // 		LGTrackLength += LightGuideHit->GetStepLength(); //this is the integrated optical photon track length from quartz exit to the cathode
    // 		LGSteps++;  //total number of steps the photon took in the light guide to get to the cathode

    // 		analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideHitTrackID((Int_t)trackID);
    // 		analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuidePhotonEnergy((Float_t)LightGuideHit->GetPhotonEnergy()/eV);
    // 		analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideStepLength((Float_t)LightGuideHit->GetStepLength()/cm);
    // 		analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideIncidentPhotonAngle((Float_t)LightGuideHit->GetIncidentPhotonAngle());
    // 		analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideReflectionProcess((Int_t)LightGuideHit->GetReflectionProcess());
		
    // 		ctrackID = trackID;
    // 		wvl = 1239.842/(LightGuideHit->GetPhotonEnergy()/eV);
    // 		LightGuideSecOptPhotonCnt->Fill(wvl,1.0/(bwdt));

    // 	      }
    // 	    }
    // 	  }
    // 	}
    // 	for(int n = 1; n <= LightGuideSecOptPhotonCnt->GetNbinsX(); n++)
    // 	  analysis->AddToAverageLightGuideOptPhotonDist(LightGuideSecOptPhotonCnt->GetBinCenter(n),LightGuideSecOptPhotonCnt->GetBinContent(n));	  
	
    // 	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideTrackData((Float_t)LGTrackLength/cm, (Int_t)LGSteps);

    //   }
    // }

    // ctrackID = -1;
    // if(QuartzHC){
      
    //   hitCnt3 = QuartzHC->entries();
    //   G4cout << "hitCnt3 = " << hitCnt3 << G4endl;
    //   if(hitCnt3){
	      
    // 	for(int j = 0; j < hitCnt3; j++){ //Note that each step inside a volume is a hit here    
    // 	  QuartzHit = (*QuartzHC)[j];
    // 	  if(QuartzHit){

    // 	    trackID = QuartzHit->GetTrackID();

    // 	    if(ctrackID != trackID){

    // 	      if(QuartzHit->GetParticleType() == 10){ //photon		  
		      
    // 		QuartzTrackLength += QuartzHit->GetStepLength(); //this is the integrated optical photon track length from creation to the exit of the quartz
    // 		QSteps++;  //total number of steps the photon took to get to the quartz exit
		
    // 		analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzHitTrackID(trackID);
    // 		analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzPhotonEnergy((Float_t)QuartzHit->GetPhotonEnergy()/eV);
    // 		//analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddParticleType(10);
    // 		analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzStepLength((Float_t)QuartzHit->GetStepLength()/cm);
    // 		ctrackID = trackID;
    // 		wvl = 1239.842/(QuartzHit->GetPhotonEnergy()/eV);
    // 		QuartzSecOptPhotonCnt->Fill(wvl,1.0/(bwdt));
    // 	      }
    // 	    }
    // 	  }	
    // 	}

    // 	for(int n = 1; n <= QuartzSecOptPhotonCnt->GetNbinsX(); n++)
    // 	  analysis->AddToAverageQuartzOptPhotonDist(QuartzSecOptPhotonCnt->GetBinCenter(n),QuartzSecOptPhotonCnt->GetBinContent(n));	  
	
    // 	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzTrackData((Float_t)QuartzTrackLength/cm, (Int_t)QSteps);

    //   }
    // }








































    // if(QuartzHC){

    //   if(LightGuideHC){
	
    // 	if(PMTHC){

    // 	  hitCnt1 = PMTHC->entries();

    // 	  for(int i = 0; i < hitCnt1; i++){ //Note that each step inside a cell is a hit here

    // 	    LGTrackLength = 0;
    // 	    QuartzTrackLength = 0;
    // 	    TotalTrackLength = 0;
    // 	    QSteps = 0;
    // 	    LGSteps = 0;
    // 	    TSteps = 0;
	    
    // 	    PMTHit = (*PMTHC)[i];
    // 	    if(PMTHit){
	      
    // 	      trackID = PMTHit->GetTrackID();
	    
    // 	      analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddTrackID(trackID);
    // 	      if(ctrackID != trackID){
		
    // 		analysis->MOLLERMainEvent->MOLLERDetectorEvent.SetPhotonEnergy((Float_t)PMTHit->GetPhotonEnergy()/eV);
    // 		analysis->MOLLERMainEvent->MOLLERDetectorEvent.SetPMTHitPositionX((Float_t)PMTHit->GetHitLocationX()/cm);
    // 		analysis->MOLLERMainEvent->MOLLERDetectorEvent.SetPMTHitPositionY((Float_t)PMTHit->GetHitLocationY()/cm);
    // 		ctrackID = trackID;
    // 	      }
	      
	      
    // 	      wvl = 1239.842/(PMTHit->GetPhotonEnergy()/eV);
    // 	      SecOptPhotonCnt->Fill(wvl,1.0/(bwdt));
	      
    // 	      hitCnt2 = LightGuideHC->entries();
	      
    // 	      if(hitCnt2){
		
    // 		for(int j = 0; j < hitCnt2; j++){ //Note that each step inside a volume is a hit here    
    // 		  LightGuideHit = (*LightGuideHC)[j];
    // 		  if(LightGuideHit){
		    
    // 		    if(LightGuideHit->GetParticleType() == 10){ //photon		  
		      
    // 		      if(LightGuideHit->GetTrackID() == trackID){
			
    // 			LGTrackLength += LightGuideHit->GetStepLength(); //this is the integrated optical photon track length from quartz exit to the cathode
    // 			//TotalTrackLength += LGTrackLength;
    // 			LGSteps++;  //total number of steps the photon took in the light guide to get to the cathode
    // 			TSteps++;
    // 			analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideStepLength((Float_t)LightGuideHit->GetStepLength()/cm);
    // 			analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideIncidentPhotonAngle((Float_t)LightGuideHit->GetIncidentPhotonAngle());
    // 			analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideReflectionProcess((Int_t)LightGuideHit->GetReflectionProcess());		
    // 		      }
    // 		    }
    // 		  }
    // 		}
    // 	      }

    // 	      hitCnt3 = QuartzHC->entries();
    // 	      if(hitCnt3){
	      
    // 		for(int j = 0; j < hitCnt3; j++){ //Note that each step inside a volume is a hit here    
    // 		  QuartzHit = (*QuartzHC)[j];
    // 		  if(QuartzHit){
		    
    // 		    if(QuartzHit->GetParticleType() == 10){ //photon		  
		      
    // 		      if(QuartzHit->GetTrackID() == trackID){
			
    // 			QuartzTrackLength += QuartzHit->GetStepLength(); //this is the integrated optical photon track length from creation to the exit of the quartz
    // 			//TotalTrackLength += QuartzTrackLength;
    // 			QSteps++;  //total number of steps the photon took to get to the quartz exit
    // 			TSteps++;
    // 			analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddParticleType(10);
    // 			analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzStepLength((Float_t)QuartzHit->GetStepLength()/cm);
			
    // 		      }
    // 		    }
    // 		  }
    // 		}	
    // 	      }
    // 	      TotalTrackLength = LGTrackLength + QuartzTrackLength;
	      
    // 	      analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddTrackData((Float_t)(LGTrackLength)/cm,(Float_t)(QuartzTrackLength)/cm, (Float_t)(TotalTrackLength)/cm,LGSteps,QSteps,TSteps);
	  
    // 	    }
    // 	  }

    // 	  for(int n = 1; n <= SecOptPhotonCnt->GetNbinsX(); n++)
    // 	    analysis->AddToAverageOptPhotonDist(SecOptPhotonCnt->GetBinCenter(n),SecOptPhotonCnt->GetBinContent(n));	  
    // 	}
    //   }

 
      
    // }






  // // if(HCE){

  // //   MOLLEROptQuartzHitsCollection* QuartzHC;
  // //   MOLLEROptLightGuideHitsCollection* LightGuideHC;
  // //   MOLLEROptPMTHitsCollection* PMTHC;
  // //   MOLLEROptQuartzHit* QuartzHit;
  // //   MOLLEROptLightGuideHit* LightGuideHit;
  // //   MOLLEROptPMTHit* PMTHit;
    
  // //   QuartzHC = (MOLLEROptQuartzHitsCollection*)(HCE->GetHC(CollID[0]));
  // //   LightGuideHC = (MOLLEROptLightGuideHitsCollection*)(HCE->GetHC(CollID[1]));
  // //   PMTHC = (MOLLEROptPMTHitsCollection*)(HCE->GetHC(CollID[2]));
    
  // //   if(QuartzHC && LightGuideHC && PMTHC){

  // //     hitCnt1 = PMTHC->entries();
  // //     hitCnt2 = LightGuideHC->entries();
  // //     hitCnt3 = QuartzHC->entries();
  // //     QSteps = 0;
      
  // //     for(int n = 0; n < hitCnt3; n++){ //loop over quartz hits first
	
  // // 	QuartzHit = (*QuartzHC)[n];
  // // 	if(QuartzHit){
	  
  // // 	  ctrackID = QuartzHit->GetTrackID();
	  
  // // 	  if(QuartzHit->GetParticleType() == 1){ //this is an electron

  // // 	    NumSecPhotons = QuartzHit->GetSecondaryPhotonVectorLength();
  // // 	    analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzStepNPhotons((Int_t)NumSecPhotons);
  // // 	    // analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddElectronTrackID((Int_t)ctrackID);
  // // 	    //analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddSecondary((Int_t)ctrackID);
	    
  // // 	    for(int c = 0; c < NumSecPhotons; c++){
	      
  // // 	      wvl = 1239.842/(QuartzHit->GetSecondaryPhotonEnergyAtIndex(c)/eV);
  // // 	      QuartzSecOptPhotonCnt->Fill(wvl,1.0/(bwdt));
	      
  // // 	    }
  // // 	  }
  // // 	  else if(QuartzHit->GetParticleType() == 10){ //photon

  // // 	    // analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzStepLength((Float_t)QuartzHit->GetStepLength()/cm);	      
  // // 	    //analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzPhotonEnergy((Float_t)QuartzHit->GetPhotonEnergy()/eV);
  // // 	    // AddQuartzTrackStep(ctrackID,QuartzHit->GetStepLength()/cm)	    
    
  // // 	  }
  // // 	}
  // //     }
  // //     for(int j = 1; j <= QuartzSecOptPhotonCnt->GetNbinsX(); j++)
  // // 	analysis->AddToAverageQuartzOptPhotonDist(QuartzSecOptPhotonCnt->GetBinCenter(j),QuartzSecOptPhotonCnt->GetBinContent(j));
  // //     QuartzSecOptPhotonCnt->Reset();
     
      
  // //     // for(int j = 0; j < ProcessedTrack.size(); j++){
  // //     // 	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzTrackData((Float_t)ProcessedTrack[j]->Length, (Int_t)ProcessedTrack[j]->Steps);

  // //     // analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzTrackID((Int_t)ProcessedTrack[j]->ID);
  // //   // }


      
  // //     // if(!TrackHasBeenProcessed(ctrackID)){
  // //     //   ProcessedTrack.push_back(ctrackID);
      
      
  // //     //   QuartzTrackLength = 0;
  // // 	    //   QSteps = 0;
  // // 	    //   for(int j = 0; j < hitCnt3; j++){ //loop over quartz hits again, this time to add data for each secondary photon
		
  // // 	    // 	QuartzHit = (*QuartzHC)[j];
  // // 	    // 	if(QuartzHit){
		  
  // // 	    // 	  if(QuartzHit->GetParticleType() == 10){ //this is a photon
		    
  // // 	    // 	    // G4cout << "Sec ID " << ctrackID << " Hit ID = " << QuartzHit->GetTrackID() << G4endl;
  // // 	    // 	    if(QuartzHit->GetTrackID() == ctrackID){
  // // 	    // 	      QuartzTrackLength += QuartzHit->GetStepLength(); 
  // // 	    // 	      QSteps++;  //total number of steps the photon took to get to the quartz exit

  // // 	    // 	    }
  // // 	    // 	  }
  // // 	    // 	}
  // // 	    //   }
  // // 	    //   if(QSteps)
  // // 	    // 	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzTrackHit(1);
  // // 	    //   else
  // // 	    // 	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzTrackHit(0);
  // // 	    //   analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddQuartzTrackData((Float_t)QuartzTrackLength/cm, (Int_t)QSteps);
	      
	      

  // // 	    //   LGTrackLength = 0;
  // // 	    //   LGSteps = 0;
  // // 	    //   for(int j = 0; j < hitCnt2; j++){ 

  // // 	    // 	LightGuideHit = (*LightGuideHC)[j];
  // // 	    // 	if(LightGuideHit){
		  
  // // 	    // 	  if(LightGuideHit->GetParticleType() == 10){ //this is a photon
		    
  // // 	    // 	    if(LightGuideHit->GetTrackID() == ctrackID){
  // // 	    // 	      LGTrackLength += LightGuideHit->GetStepLength(); 
  // // 	    // 	      LGSteps++;  //total number of steps the photon took to get to the light guide exit
  // // 	    // 	      analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuidePhotonEnergy((Float_t)LightGuideHit->GetPhotonEnergy()/eV);
  // // 	    // 	      analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideStepLength((Float_t)LightGuideHit->GetStepLength()/cm);
  // // 	    // 	      analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideIncidentPhotonAngle((Float_t)LightGuideHit->GetIncidentPhotonAngle());
  // // 	    // 	      analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideReflectionProcess((Int_t)LightGuideHit->GetReflectionProcess());
  // // 	    // 	      wvl = 1239.842/(LightGuideHit->GetPhotonEnergy()/eV);
  // // 	    // 	      LightGuideSecOptPhotonCnt->Fill(wvl,1.0/(bwdt));		      
  // // 	    // 	    }
  // // 	    // 	  }
  // // 	    // 	}
  // // 	    //   }
  // // 	    //   if(LGSteps)
  // // 	    // 	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideTrackHit(1);
  // // 	    //   else
  // // 	    // 	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideTrackHit(0);
  // // 	    //   analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddLightGuideTrackData((Float_t)LGTrackLength/cm, (Int_t)LGSteps);


  // // 	    //   hitflag = 0;
  // // 	    //   for(int j = 0; j < hitCnt1; j++){ 

  // // 	    // 	PMTHit = (*PMTHC)[j];
  // // 	    // 	if(PMTHit){
		  
  // // 	    // 	  if(PMTHit->GetParticleType() == 10){ //this is a photon

  // // 	    // 	    if(PMTHit->GetTrackID() == ctrackID){
  // // 	    // 	      analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddPMTPhotonEnergy((Float_t)PMTHit->GetPhotonEnergy()/eV);
  // // 	    // 	      analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddPMTHitPositionX((Float_t)PMTHit->GetHitLocationX()/cm);
  // // 	    // 	      analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddPMTHitPositionY((Float_t)PMTHit->GetHitLocationY()/cm);
  // // 	    // 	      wvl = 1239.842/(PMTHit->GetPhotonEnergy()/eV);
  // // 	    // 	      PMTSecOptPhotonCnt->Fill(wvl,1.0/(bwdt));
  // // 	    // 	      hitflag = 1;
  // // 	    // 	    }
  // // 	    // 	  }
  // // 	    // 	}
  // // 	    //   }
  // // 	    //   if(hitflag)
  // // 	    // 	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddPMTTrackHit(1);
  // // 	    //   else
  // // 	    // 	analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddPMTTrackHit(0);


  // // 	    //   for(int j = 1; j <= LightGuideSecOptPhotonCnt->GetNbinsX(); j++)
  // // 	    // 	analysis->AddToAverageLightGuideOptPhotonDist(LightGuideSecOptPhotonCnt->GetBinCenter(j),LightGuideSecOptPhotonCnt->GetBinContent(j));	  
  // // 	    //   LightGuideSecOptPhotonCnt->Reset();
	      
  // // 	    //   for(int j = 1; j <= PMTSecOptPhotonCnt->GetNbinsX(); j++)
  // // 	    // 	analysis->AddToAveragePMTOptPhotonDist(PMTSecOptPhotonCnt->GetBinCenter(j),PMTSecOptPhotonCnt->GetBinContent(j));	  
  // // 	    //   PMTSecOptPhotonCnt->Reset();
   
  // // 	    // }


  // //   }
 




    
  // //   // TotalTrackLength = LGTrackLength + QuartzTrackLength;
    
  // //   // analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddTrackData((Float_t)(LGTrackLength)/cm,(Float_t)(QuartzTrackLength)/cm, (Float_t)(TotalTrackLength)/cm,LGSteps,QSteps,TSteps);

	    
  // //   //Note: When calculating the distribution of CHerenkov photons emitted per dx per wavelength, don't
  // //   //use the step length itself as dx. Instead use the total track length of the particle in a given 
  // //   //volume. The reason is that the steps G4 takes get smaller and smaller toward a volume boundary and
  // //   //the emitted photons seem to be more biased toward higher wavelengths, which causes the resulting 
  // //   //distribution to be biased toward higher wavelengths.
  // //   // if(hitCnt2){
    
  // //   // 	SecOptPhotonCnt->Reset();
    
  // //   // 	for(int j = 0; j < hitCnt2; j++){ //Note that each step inside a cell is a hit here    
  // //   // 	  detHit = (*DetectorHC)[j];
  // //   // 	  if(detHit){
    
  // //   // 	    if(detHit->GetParticleType() == 1){ //electron
    
  // //   // 	      analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddStepData((Float_t)detHit->GetStepLength()/cm);
  // //   // 	      analysis->MOLLERMainEvent->MOLLERDetectorEvent.AddParticleType(1);
    
  // //   // 	      secPhCnt = detHit->GetSecondaryPhotonVectorLength();
  // //   // 	      trackLength += detHit->GetStepLength()/cm;
    
  // //   // 	      for(int k = 0; k < secPhCnt; k++){
    
  // //   // 		optPhEng = (Float_t)detHit->GetSecondaryPhotonEnergyAtIndex(k);
  // //   // 		wvl = 1239.842/optPhEng;
  // //   // 		SecOptPhotonCnt->Fill(wvl,1.0/(bwdt));
  // //   // 	      }
  // //   // 	    }
  // //   // 	  }
  // //   // 	}
  // //   // 	for(int n = 1; n <= SecOptPhotonCnt->GetNbinsX(); n++)
  // //   // 	  analysis->AddToAverageOptPhotonDist(SecOptPhotonCnt->GetBinCenter(n),SecOptPhotonCnt->GetBinContent(n)/trackLength);	  
	
  // //   // }
    
  // //   OriginVertexX = (Float_t)(evt->GetPrimaryVertex(0)->GetPosition().x()/cm);
  // //   OriginVertexY = (Float_t)(evt->GetPrimaryVertex(0)->GetPosition().y()/cm);
  // //   OriginVertexZ = (Float_t)(evt->GetPrimaryVertex(0)->GetPosition().z()/cm);
    
  // //   analysis->MOLLERMainEvent->MOLLERPrimEvent.StoreOriginVertexPositionX(OriginVertexX);
  // //   analysis->MOLLERMainEvent->MOLLERPrimEvent.StoreOriginVertexPositionY(OriginVertexY);
  // //   analysis->MOLLERMainEvent->MOLLERPrimEvent.StoreOriginVertexPositionZ(OriginVertexZ);
    
  // //   analysis->MOLLERMainEvent->MOLLERPrimEvent.StorePrimaryEventNumber((Int_t)primaryEventNumber);  	   
  // //   analysis->FillRootNtuple();
  // // }
