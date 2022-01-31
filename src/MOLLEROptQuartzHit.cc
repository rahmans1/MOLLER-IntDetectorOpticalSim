#include "MOLLEROptQuartzHit.hh"

MOLLEROptQuartzHit::MOLLEROptQuartzHit()
{
  Particle      = -1;
  StepLength = 0.;
  PhotonEnergy = 0.;
  TrackID = -1;
  SecPhotonEnergy.clear();
  SecPhotonEnergy.resize(0);
}


MOLLEROptQuartzHit::~MOLLEROptQuartzHit()
{

}
