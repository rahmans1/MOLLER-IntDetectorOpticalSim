#include "MOLLEROptDetectorEvent.hh"


//These are defined in "TrackInformation"
// #define myElectron 1
// #define myPositron 2
// #define myGamma    3  //as in non-optical photon including x-ray
// #define myProton   4
// #define myNeutron  5
// #define myTriton   6
// #define myAlpha    7
// #define myHelium3  8
// #define myNucleus  9


MOLLEROptDetectorEvent::MOLLEROptDetectorEvent() 
{
  Initialize();
} 

MOLLEROptDetectorEvent::~MOLLEROptDetectorEvent()
{ 

}

void MOLLEROptDetectorEvent::Initialize()
{
  LightGuideStepLength.clear();  
  LightGuideStepLength.resize(0);

  LightGuideIncidentPhotonAngle.clear();
  LightGuideIncidentPhotonAngle.resize(0);

  QuartzIncidentPhotonAngle.clear();
  QuartzIncidentPhotonAngle.resize(0);

  QuartzSecondaryPhotonAngle.clear();
  QuartzSecondaryPhotonAngle.resize(0);

  QuartzStepLength.clear();  
  QuartzStepLength.resize(0);
  
  TotalTrackLength.clear();  
  TotalTrackLength.resize(0);

  // QuartzTrackLength.clear();  
  // QuartzTrackLength.resize(0);

  QuartzTrackLength = 0;

  LightGuideTrackLength.clear();  
  LightGuideTrackLength.resize(0);
  
  TotalSteps.clear();  
  TotalSteps.resize(0);

  // QuartzSteps.clear();  
  // QuartzSteps.resize(0);

  QuartzSteps = 0;

  LightGuideSteps.clear();  
  LightGuideSteps.resize(0);

  PMTTrackHit.clear();  
  PMTTrackHit.resize(0);

  //PMTTrackHit = 0;

  LightGuideTrackHit.clear();  
  LightGuideTrackHit.resize(0);

  QuartzTrackHit.clear();  
  QuartzTrackHit.resize(0);

  QuartzHitX.clear();  
  QuartzHitX.resize(0);

  QuartzHitY.clear();  
  QuartzHitY.resize(0);

  PMTPhotonEnergy.clear();  
  PMTPhotonEnergy.resize(0);

  // QuartzPhotonEnergy.clear();  
  // QuartzPhotonEnergy.resize(0);

  QuartzPhotonEnergy = 0;

  QuartzStepNPhotons.clear();
  QuartzStepNPhotons.resize(0);
  
  LightGuidePhotonEnergy.clear();  
  LightGuidePhotonEnergy.resize(0);

  // ElectronTrackID.clear();
  // ElectronTrackID.resize(0);

  QuartzElectronStepLength.clear();
  QuartzElectronStepLength.resize(0);

  ElectronTrackID = 0;
  PhotonTrackID = 0;
  
  PType.clear();  
  PType.resize(0);

  PMTCathodeHitX.clear();  
  PMTCathodeHitX.resize(0);

  PMTCathodeHitY.clear();  
  PMTCathodeHitY.resize(0);

  PMTWindowReflectionAngle.clear();  
  PMTWindowReflectionAngle.resize(0);

  PMTAnodePulse.clear();
  PMTAnodePulse.resize(0);

}	


void MOLLEROptDetectorEvent::AddQuartzTrackData(Float_t QTrackL, Int_t QSteps)
{
  QuartzTrackLength = QTrackL;  
  QuartzSteps = QSteps;  
  // QuartzTrackLength.push_back(QTrackL);  
  // QuartzSteps.push_back(QSteps);  
}

void MOLLEROptDetectorEvent::AddLightGuideTrackData(Float_t LGTrackL, Int_t LGSteps)
{
  LightGuideTrackLength.push_back(LGTrackL);
  LightGuideSteps.push_back(LGSteps);  
}

// void MOLLEROptDetectorEvent::AddTrackPMTSignal(Float_t TrackL, Float_t eng)
// {
  
  
//   LightGuideTrackLength.push_back(LGTrackL);
//   LightGuideSteps.push_back(LGSteps);  
// }

