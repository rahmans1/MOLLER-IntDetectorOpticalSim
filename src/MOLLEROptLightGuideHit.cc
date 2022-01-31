#include "MOLLEROptLightGuideHit.hh"

MOLLEROptLightGuideHit::MOLLEROptLightGuideHit()
{
  Particle      = -1;
  StepLength = 0.;
  PhotonEnergy = 0.;
  TrackID = -1;
  SecPhotonEnergy.clear();
  SecPhotonEnergy.resize(0);
}


MOLLEROptLightGuideHit::~MOLLEROptLightGuideHit()
{

}

