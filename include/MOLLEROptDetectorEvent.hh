#ifndef MOLLEROptDetectorEvent_h
#define MOLLEROptDetectorEvent_h

#include "cpp_include.h"
#include "Root_include.h"

class MOLLEROptDetectorEvent : public TObject
{

private:

  vector <Int_t> PType;
  // vector <Int_t> ElectronTrackID;
  // vector <Int_t> PhotonTrackID;
  Int_t ElectronTrackID;
  Int_t PhotonTrackID;
  Int_t ParentID;
  Int_t EventID;

  Float_t InitialTrackMomDirectionX;
  Float_t InitialTrackMomDirectionY;
  Float_t InitialTrackMomDirectionZ;
  
  // vector <Int_t> PMTTrackHit;
  Int_t PMTTrackHit;
  
  vector <Float_t> PMTPhotonEnergy;
  vector <Float_t> PMTCathodeHitX;
  vector <Float_t> PMTCathodeHitY;
  vector <Float_t> PMTWindowReflectionAngle;
  
  vector <Int_t> LightGuideTrackHit;
  vector <Float_t> LightGuidePhotonEnergy;
  vector <Int_t> LightGuideSteps;
  vector <Float_t> LightGuideStepLength;
  vector <Float_t> LightGuideTrackLength;
  vector <Float_t> LightGuideIncidentPhotonAngle;
  vector <Int_t> LightGuideReflectionProcess;

  vector <Int_t> QuartzTrackHit;
  vector <Float_t> QuartzHitX;
  vector <Float_t> QuartzHitY;
  // vector <Float_t> QuartzPhotonEnergy;
  Float_t QuartzPhotonEnergy;
  vector <Float_t> QuartzSecondaryPhotonAngle;
  // vector <Int_t> QuartzSteps;
  Int_t QuartzSteps;
  vector <Float_t> QuartzStepLength;
  vector <Float_t> QuartzElectronStepLength;
  vector <Float_t> QuartzIncidentPhotonAngle;
  // vector <Float_t> QuartzTrackLength;
  Float_t QuartzTrackLength;
  vector <Int_t>  QuartzStepNPhotons;
  // vector <Int_t>  QuartzPhotonAtExit;
  Int_t  QuartzPhotonAtExit;

  vector <Int_t> TotalSteps;
  vector <Float_t> TotalTrackLength;  
  

public:

  
  MOLLEROptDetectorEvent();
  virtual ~MOLLEROptDetectorEvent();

  void Initialize();

  void AddLightGuidePhotonEnergy(Float_t eng) {LightGuidePhotonEnergy.push_back(eng);};
  void AddLightGuideTrackHit(Int_t tID){LightGuideTrackHit.push_back(tID);};
  void AddLightGuideStepLength(Float_t dx){ LightGuideStepLength.push_back(dx);};
  void AddLightGuideIncidentPhotonAngle(Float_t angle) {LightGuideIncidentPhotonAngle.push_back(angle);};
  void AddLightGuideReflectionProcess(Int_t proc){LightGuideReflectionProcess.push_back(proc);};
  void AddLightGuideTrackData(Float_t LGTrackL, Int_t LGSteps);

  // void AddQuartzPhotonEnergy(Float_t eng) {QuartzPhotonEnergy.push_back(eng);};
  void AddQuartzPhotonEnergy(Float_t eng) {QuartzPhotonEnergy = eng;};
  void AddQuartzTrackHit(Int_t tID){QuartzTrackHit.push_back(tID);};
  void AddQuartzStepLength(Float_t dx){ QuartzStepLength.push_back(dx);};
  void AddQuartzElectronStepLength(Float_t dx) {QuartzElectronStepLength.push_back(dx);};
  void AddQuartzIncidentPhotonAngle(Float_t angle) {QuartzIncidentPhotonAngle.push_back(angle);};
  void AddQuartzTrackData(Float_t QTrackL, Int_t QSteps);
  void AddQuartzStepNPhotons(Int_t n){QuartzStepNPhotons.push_back(n);};
  // void AddQuartzPhotonAtExitFlag(Int_t flag){QuartzPhotonAtExit.push_back(flag);};
  void AddQuartzPhotonAtExitFlag(Int_t flag){QuartzPhotonAtExit = flag;};
  void AddQuartzTrackSecPhotonAngle(Float_t angle) {QuartzSecondaryPhotonAngle.push_back(angle);}

  // void AddElectronTrackID(Int_t tID){ElectronTrackID.push_back(tID);};
  // void AddPhotonTrackID(Int_t tID){PhotonTrackID.push_back(tID);};
  void AddElectronTrackID(Int_t tID){ElectronTrackID = tID;};
  void AddPhotonTrackID(Int_t tID){PhotonTrackID = tID;};

  // void AddParticleType(Int_t pTyp) {PType.push_back(pTyp);};

  void AddPMTPhotonEnergy(Float_t eng) {PMTPhotonEnergy.push_back(eng);};
  // void AddPMTTrackHit(Int_t tID){PMTTrackHit.push_back(tID);};
  void AddPMTTrackHit(Int_t tID){PMTTrackHit = tID;};
  void AddPMTHitPositionX(Float_t x) {PMTCathodeHitX.push_back(x);};
  void AddPMTHitPositionY(Float_t y) {PMTCathodeHitY.push_back(y);};
  void AddQuartzHitPositionX(Float_t x) {QuartzHitX.push_back(x);};
  void AddQuartzHitPositionY(Float_t y) {QuartzHitY.push_back(y);};
  void AddPMTWindowReflectionAngle(Float_t ang) {PMTWindowReflectionAngle.push_back(ang);};

  void AddTrackInitMomDirection(Float_t x, Float_t y, Float_t z) {
    InitialTrackMomDirectionX = x;
    InitialTrackMomDirectionY = y;
    InitialTrackMomDirectionZ = z;
  };
  void SetTrackParentID(Int_t ID) {ParentID = ID;};
  void SetEventID(Int_t ID) {EventID = ID;};
  
  ClassDef(MOLLEROptDetectorEvent,1)

};

#endif 
