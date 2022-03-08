#include "MOLLEROptAnalysis.hh"

MOLLEROptAnalysis::MOLLEROptAnalysis(MOLLEROptConstruction *cnstr)
{	
  // Initialize 

  Construction = cnstr;
  MOLLERMainEvent    = NULL;
  MOLLEROptMainBranch   = NULL;
  
  MOLLEROptNtuple         = NULL;
  MOLLEROptFile           = NULL;
  NumberOfPrimaries     = new TVectorD(1);
  NumberOfPrimaries[0]  = 0;
  ROOTFileFlag = 1;
  
  // for(int n = 0; n < 800; n++){
  //   OptPhotonDist[n] = 0.;
  //   OptPhotonDistCnt[n] = 0;
  // }
  PMTOptPhotonDistrHist = new TProfile("PMTOptPhotonDistrHist","",800,100,900);
  QuartzOptPhotonDistrHist = new TProfile("QuartzOptPhotonDistrHist","",800,100,900);
  LightGuideOptPhotonDistrHist = new TProfile("LightGuideOptPhotonDistrHist","",800,100,900);
  PhotoElectronDistrHist = new TH1D("PhotoElectronDistrHist","",100,0,100);
  CathodeEventsDistrHist = new TH1D("CathodeEventsDistrHist","",100,0,100);
  EventCnt = 0;
}

MOLLEROptAnalysis::~MOLLEROptAnalysis() 
{
  Finish();
  delete[] NumberOfPrimaries;
}

void MOLLEROptAnalysis::Init()
{;}


void MOLLEROptAnalysis::Finish()
{
  if (MOLLERMainEvent)         delete MOLLERMainEvent;
  if (MOLLEROptMainBranch)     delete MOLLEROptMainBranch;
  if (MOLLEROptNtuple)         {MOLLEROptNtuple->Delete();}
  if (MOLLEROptFile)           delete MOLLEROptFile;
}   

void MOLLEROptAnalysis::BeginOfRun(G4int runID, G4String runFileName,MOLLEROptTrackingReadout *TrRO) 
{   

  if(ROOTFileFlag){

    MOLLEROptFile = new TFile(Form("MOLLEROpt%s_%04d.root",runFileName.data(),runID),"RECREATE","MOLLEROpt ROOT file");
  }
  TrackingReadout = TrRO;
    
  ConstructRootNtuple();
}

void MOLLEROptAnalysis::EndOfRun() 
{
  if(ROOTFileFlag){
    NumberOfPrimaries->Write("NumberOfPrimaries");
    //FillOptPhotonDistrHist();
    PMTOptPhotonDistrHist->Write();
    QuartzOptPhotonDistrHist->Write();
    LightGuideOptPhotonDistrHist->Write();
    PhotoElectronDistrHist->Write();
    CathodeEventsDistrHist->Write();
    TrackingReadout->WriteAbsProfiles();

    DesignParameters *parms = Construction->GetDetectorDesignParameters();
    TVectorD QuartzSizeX(1); 
    TVectorD QuartzSizeY(1); 
    TVectorD QuartzSizeZ(1);
    
    TVectorD LowerInterfacePlane(1);
    TVectorD UpperInterfacePlane(1);
    TVectorD LowerConeFrontFaceAngle(1);
    TVectorD LowerConeBackFaceAngle(1);
    TVectorD LowerConeSideFaceAngle(1);
    TVectorD QuartzInterfaceOpeningZ(1);
    TVectorD QuartzInterfaceOpeningX(1);
    TVectorD PMTInterfaceOpeningZ(1);
    TVectorD PMTInterfaceOpeningX(1);
    TVectorD QuartzToPMTOffsetInZ(1);
    TVectorD QuartzBevel(1);
    TVectorD HitRegion(1);

    QuartzSizeX[0] =             parms->QuartzSizeX;            ; 
    QuartzSizeY[0] =             parms->QuartzSizeY; 
    QuartzSizeZ[0] =             parms->QuartzSizeZ;
    LowerInterfacePlane[0] =     parms->LowerInterfacePlane;   
    UpperInterfacePlane[0] =     parms->UpperInterfacePlane;   
    LowerConeFrontFaceAngle[0] = parms->LowerConeFrontFaceAngle/degree;
    LowerConeBackFaceAngle[0] =  parms->LowerConeBackFaceAngle/degree;
    LowerConeSideFaceAngle[0] =  parms->LowerConeSideFaceAngle/degree;
    QuartzInterfaceOpeningZ[0] = parms->QuartzInterfaceOpeningZ;
    QuartzInterfaceOpeningX[0] = parms->QuartzInterfaceOpeningX;
    PMTInterfaceOpeningZ[0] =    parms->PMTInterfaceOpeningZ;  
    PMTInterfaceOpeningX[0] =    parms->PMTInterfaceOpeningX;  
    QuartzToPMTOffsetInZ[0] =    parms->QuartzToPMTOffsetInZ;  
    QuartzBevel[0] =             parms->QuartzBevel;
    HitRegion[0] =               Construction->GetEventHitRegion();
    
    QuartzSizeX.Write("QuartzSizeX");            
    QuartzSizeY.Write("QuartzSizeY");            
    QuartzSizeZ.Write("QuartzSizeZ");            
                            
    LowerInterfacePlane.Write("LowerInterfacePlane");    
    UpperInterfacePlane.Write("UpperInterfacePlane");    
    LowerConeFrontFaceAngle.Write("LowerConeFrontFaceAngle");
    LowerConeBackFaceAngle.Write("LowerConeBackFaceAngle"); 
    LowerConeSideFaceAngle.Write("LowerConeSideFaceAngle"); 
    QuartzInterfaceOpeningZ.Write("QuartzInterfaceOpeningZ");
    QuartzInterfaceOpeningX.Write("QuartzInterfaceOpeningX");
    PMTInterfaceOpeningZ.Write("PMTInterfaceOpeningZ");  
    PMTInterfaceOpeningX.Write("PMTInterfaceOpeningX");  
    QuartzToPMTOffsetInZ.Write("QuartzToPMTOffsetInZ");  
    QuartzBevel.Write("QuartzBevel");
    HitRegion.Write("HitRegion");
    
    MOLLEROptFile->Write("",TObject::kOverwrite); // Writing the data to the ROOT file
    
    MOLLEROptNtuple->Reset(); //This needs to be here, so that the file size doesn't keep growing for new files.
                            //Apparently things are kept in the background, from the previous tree, and then 
                            //written to the new tree, without it being visible or accessible in the new file.
                            //This is only happening or a concern if multiple ROOT files are written in the same
                            //execution of the simulation, but it is a collossally stupid attribute of ROOT.

    MOLLEROptFile->Close();
  }

  if (MOLLERMainEvent)         delete MOLLERMainEvent;
  if (MOLLEROptFile)           delete MOLLEROptFile;
  
}

void MOLLEROptAnalysis::EndOfEvent(G4int flag) 
{
  if(!flag) return;
}

void MOLLEROptAnalysis::ConstructRootNtuple() 
{
  MOLLERMainEvent   = new MOLLEROptMainEvent();

  if(ROOTFileFlag) {
    MOLLEROptNtuple = new TTree("MOLLEROptTree","MOLLEROptTree");
    MOLLEROptMainBranch  = MOLLEROptNtuple->Branch("MOLLEROptData", "MOLLEROptMainEvent", &MOLLERMainEvent, 64000, 10);
    //MOLLEROptNtuple->SetMaxVirtualSize(10000000);
  }
}

void MOLLEROptAnalysis::AutoSaveRootNtuple() 
{
    // save the current ntuple:
    // In case your program crashes before closing the file holding this tree,
    // the file will be automatically recovered when you will connect the file
    // in UPDATE mode.
    // The Tree will be recovered at the status corresponding to the last AutoSave.
    //
    // if option contains "SaveSelf", gDirectory->SaveSelf() is called.
    // This allows another process to analyze the Tree while the Tree is being filled.
    //
    // see http://root.cern.ch/root/html/TTree.html#TTree:AutoSave

    //MOLLEROptG4_RootNtuple -> AutoSave("SaveSelf");
  if(ROOTFileFlag) {
    Long64_t bytesWritten = MOLLEROptNtuple->AutoSave("FlushBaskets");
    //cout << "\nBytes written to file: " << bytesWritten << endl;
  }
}
