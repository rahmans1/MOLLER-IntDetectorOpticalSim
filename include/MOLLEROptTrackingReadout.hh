#ifndef MOLLEROptTrackingReadout_h
#define MOLLEROptTrackingReadout_h 

#include "cpp_include.h"
#include "Root_include.h"
#include "Geant4_include.hh" 

#define myElectron 1
#define myPositron 2
#define myGamma    3  //as in non-optical photon including x-ray
#define myPhoton   4  //as in optical photon

#define myQuartz 10
#define myLightGuide 11
#define myPMT 12

class OptParam;

class TrackData{

public:
  Int_t ID;
  Int_t ParentID;
  Int_t Particle;
  Int_t Length;
  Int_t QLength;
  Int_t LGLength;
  Int_t NSteps;
  Int_t QSteps;
  Int_t LGSteps;
  Int_t QExitFlag;
  Int_t PMTHitFlag;
  Int_t QuartzHitFlag;
  Int_t LGHitFlag;
  Int_t Detected;
  Float_t InitEnergy;
  Float_t InitKinEnergy;
  Float_t InitWavelength;
  Float_t InitMomDirX;
  Float_t InitMomDirY;
  Float_t InitMomDirZ;
  Float_t PMTHitX;
  Float_t PMTHitY;
  Float_t PMTHitZ;
  Float_t PMTWinRefl;
  Float_t QuartzHitX;
  Float_t QuartzHitY;
  Float_t QuartzHitZ;
  vector <Float_t> StepLength;
  vector <Float_t> StepAngle;  
  vector <Float_t> SecPhotonAngle;  
  vector <Float_t> SecPhotonWavelength;  
  vector <Int_t> StepVolume;
  vector <Int_t> StepDefProc;
  vector <Int_t> StepNChPhotons;
  vector <Float_t> StepEnergy;
  vector <Float_t> StepWavelength;
};


class MOLLEROptTrackingReadout 
{
public:
  
   MOLLEROptTrackingReadout();
  ~MOLLEROptTrackingReadout();

private:

  vector <TrackData*> Tracks;
  Int_t ElectronTracks;
  Int_t PhotonTracks;
  Int_t CathodeDetections;

  TH2D *DetectorRateGlobal;
  TH2D *DetectorRateGlobalRotated;
  TH2D *DetectorRateLocalRotated;
  TH2D *DetectorRateLocalRotatedCoarse;
  TH1D *QEfficiency;

  TProfile* Transm35;
  TProfile* Transm70;
  TProfile* Transm170;
  TProfile* Reflectivity;
  
  TH1D *LGRefl30;
  TH1D *LGRefl45;
  TH1D *LGRefl60;
  TH1D *LGRefl90;

  TH1D* CathIndR;  
  TH1D* CathIndI;  

  OptParam* OpticalParameters;

  vector <TProfile*> QAbsProf; 


 public:

  void Initialize();
  void AddNewTrack(TrackData *track);
  void AddTrackData(Int_t id, Int_t type, Int_t StepL, Int_t QEFlag, Int_t PMTHit, Int_t Volume, Int_t Proc, Float_t eng, Float_t wvl, Float_t Ang);
  void AddSecPhoton(Int_t id, Float_t ang, Float_t wvl);
  void AddStepNCherenkovs(Int_t n, Int_t nsec);
  void IncrementEventCathodeDetection(Int_t ID);
  Int_t GetCathodeDetections() {return CathodeDetections;};
  void SetPMTHitLocation(Int_t id, G4ThreeVector loc, Float_t angle);
  void SetQuartzHitLocation(Int_t id, G4ThreeVector loc);
  Bool_t TrackExists(Int_t ID);
  Int_t GetNumTracks(){return Tracks.size();};
  Int_t GetNumPhotonTracks(){return PhotonTracks;};
  Int_t GetNumElectronTracks(){return ElectronTracks;};
  TrackData* GetTrackData(Int_t n) {if(n >= 0 && n < Tracks.size()) return Tracks[n]; return 0;};
  Int_t GetStepInQuartz(Int_t TID);
  void SetRadiationDamageHistograms(TH2 *hst, TH2 *hst2, TH2 *hst3, TH2 *hst4);
  void StoreQuartzOpticalPlots(TProfile*,TProfile*,TProfile*,TProfile*,TH1D*);
  void StoreGuideOpticalPlots(TH1D*,TH1D*,TH1D*,TH1D*);
  void StoreCathodeOpticalPlots(TH1D*, TH1D*);
  void SetNewQAbsorptionProfile(TProfile* abs);
  void WriteAbsProfiles();
  void SetOpticalParameters(OptParam* op){ OpticalParameters = op;};
  OptParam* GetOpticalParameters(){ return OpticalParameters;};
  
  
};

#endif
