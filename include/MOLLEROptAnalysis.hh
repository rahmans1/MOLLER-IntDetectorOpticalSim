#ifndef MOLLEROptAnalysis_h 
#define MOLLEROptAnalysis_h 

#include "cpp_include.h"
#include "Root_include.h"
#include "Geant4_include.hh" 

#include "MOLLEROptMaterial.hh"
#include "MOLLEROptMainEvent.hh"
#include "MOLLEROptTrackingReadout.hh"
#include "MOLLEROptConstruction.hh"

class MOLLEROptMainEvent;

class MOLLEROptAnalysis {
public:
  MOLLEROptAnalysis(MOLLEROptConstruction *constr);
  virtual ~MOLLEROptAnalysis();
  
public:

  void BeginOfRun(G4int runID, G4String runFileName, MOLLEROptTrackingReadout *TrRO);
  void EndOfRun();
  void EndOfEvent(G4int flag);

  void Init();
  void Finish();

  void FillRootNtuple() {if(ROOTFileFlag) MOLLEROptNtuple->Fill();}
  void AutoSaveRootNtuple();

  void SetTotalNumberOfPrimaries(Int_t n) {(*NumberOfPrimaries)[0] = (Double_t)n;};
  void SetROOTFileFlag(G4int flag) {ROOTFileFlag = flag;};

  void AddToAveragePMTOptPhotonDist(Float_t wvl, Float_t wgt){PMTOptPhotonDistrHist->Fill(wvl,wgt);};
  void AddToAverageQuartzOptPhotonDist(Float_t wvl, Float_t wgt){QuartzOptPhotonDistrHist->Fill(wvl,wgt);};
  void AddToAverageLightGuideOptPhotonDist(Float_t wvl, Float_t wgt){LightGuideOptPhotonDistrHist->Fill(wvl,wgt);};

  void AddPhotoElectronEvent(Double_t pe){ PhotoElectronDistrHist->Fill(pe);};
  void AddCathodeDetectionEvent(Int_t events) {CathodeEventsDistrHist->Fill(events);};

  MOLLEROptMainEvent* MOLLERMainEvent;

private:

  MOLLEROptTrackingReadout* TrackingReadout;
  MOLLEROptConstruction* Construction;
  
  void ConstructRootNtuple(); 

  TTree*   MOLLEROptNtuple;
  TBranch* MOLLEROptMainBranch;
  TFile*   MOLLEROptFile;
  TProfile*    PMTOptPhotonDistrHist;
  TProfile*    QuartzOptPhotonDistrHist;
  TProfile*    LightGuideOptPhotonDistrHist;
  TH1D*    PhotoElectronDistrHist;
  TH1D*    CathodeEventsDistrHist;
  
  TVectorD *NumberOfPrimaries;
  Float_t OptPhotonDist[800];
  Int_t OptPhotonDistCnt[800];

  G4int ROOTFileFlag;
  Int_t EventCnt;


};

#endif
