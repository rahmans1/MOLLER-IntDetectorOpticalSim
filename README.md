# Software Front-end for testing the MOLLER/P2 Integrating Electronics Prototypes 

## This was successfully compiled with

* Ubuntu 18.04.1 LTS
* 5.4.0-91-generic
* ROOT 6.22/08 with all required and recommended support programs
* Geant4 10.06
* The simulation uses a modified G4OpBoundaryProcess class, to allow evaluation of angle dependent reflectivity
* The corresponding modified class files are in the 'data' subdirectory and need to be copied to 'G4SourceDir/source/processes/optical/'  
* and G4 needs to recompiled.

## Project Layout

* /cmake
* /data
* /include
* /macros
* /source

## Setup

* 'mkdir <some directory>'
* 'cd <some directory>'
* clone the repository
* 'mkdir build'
* 'cd build'
* 'cmake ..'
* 'make'

## Running

Either
* './MOLLEROpt' (without commandline arguments - visualization interface mode)
Or
* './MOLLEROpt macros/*.mac (to run a number of events without visualization - main run mode)'


## Notes

### Macros and geometry

The following commands can presently be issued from the macro files (see included macro example for details):

Light guide geometry:

* /Det/LightGuideLowerConeSideAngle       #These are normally straight, unless the quartz is very narrow
* /Det/LightGuideLowerConeBackAngle       #Angle between positive y axis (length of quartz) and the downstram lower cones surface 
* /Det/LightGuideLowerConeFrontAngle      #Angle between positive y axis (length of quartz) and the upstram lower cones surface
* /Det/LightGuideLowerInterface           #Distance between quartz-LG interface and lower interface plane (length of lower cone)
* /Det/LightGuideUpperInterface           #Distance between quartz-LG interface and upper interface plane (length of light guide)

* /Det/LightGuidePMTInterfaceOpeningX     #These should stay fixed - set by the PMT size 
* /Det/LightGuidePMTInterfaceOpeningZ     #These should stay fixed - set by the PMT siz

* /Det/LightGuideQuartzInterfaceOpeningX  #These should be set according to the quartz size (always add 8 mm to quartz width)
* /Det/LightGuideQuartzInterfaceOpeningZ  #But larger, according to the CAD model  (it's 1.7 cm for 1 cm thick quartz and 2.2 cm for 1.5 cm thick quartz)

* /Det/LightGuideQuartzToPMTOffset        #Offset of PMT cathode center along Z (beam direction) w.r.t quartz center

* /Det/QuartzSizeZ                        #Quartz thickness
* /Det/QuartzSizeX                        #Quartz width
* /Det/QuartzSizeY                        #Quartz length Without the 45 degree cut
* /Det/QuartzBevelSize                    #Should not normally have a bevel. So the normal condition is 0 mm

* /Det/QuartzRotX                         #rotate the quartz w.r.t. the light guide
* /Det/PolarRotation                      #counter rotate the detector as a whole so that the electrons are normally incidendt on quartz

* /Det/SetSegmentedRadDamageFlag          #Uncomment this to include quartz radiation damage. Segments the quartz into 2 cm bins with varying rad damange factors
* /MOLLEROpt/QuartzRadDamage              #Set peak quartz radaiation damage to 70 MRad (currently other possibility is 170)

* /Generator/EventHitRegion               #1 = quartz, 2 = lower LG cone, 3 = upper LG cone, 4 = 2x2 mm^2 spot on center of quartz, 5 = 2x2 mm^2 on center of lower guide, 6 = 2x2 mm^2 spot on upper guide

* In principle changing any of the geometry parameters should suitably adjust the other parts, but it is better to check with
a visualization run.


### Readout

The simulation data is saved in a root file that can be given a unique ID from the macro '/RunAction/SetID'. The root file contains a tree that is not electron event based, but rather based on Cherenkov photon events. So the number of track entries in the tree is much larger than than number of electron events. Various things are recorded, including:

  * 'PMTPhotonEnergy'
  * 'PMTCathodeHitX'
  * 'PMTCathodeHitY'
  * 'PMTWindowReflectionAngle'
  
  * 'LightGuideTrackHit'
  * 'LightGuidePhotonEnergy'
  * 'LightGuideSteps'
  * 'LightGuideStepLength'
  * 'LightGuideTrackLength'
  * 'LightGuideIncidentPhotonAngle'
  * 'LightGuideReflectionProcess'

  * 'QuartzTrackHit'
  * 'QuartzHitX'
  * 'QuartzHitY'

  * 'QuartzPhotonEnergy'
  * 'QuartzSecondaryPhotonAngle'
  * 'QuartzSteps'
  * 'QuartzStepLength'
  * 'QuartzElectronStepLength'
  * 'QuartzIncidentPhotonAngle'
  * 'QuartzTrackLength'
  * 'QuartzStepNPhotons'
  * 'QuartzPhotonAtExit'

  * 'TotalSteps'
  * 'TotalTrackLength'

  One can then, for example, plot:

  'tv__tree->Draw("MOLLERDetectorEvent.QuartzTrackLength:MOLLERDetectorEvent.QuartzSteps>>hist5(700,0,700,1600,0,1600)","MOLLERDetectorEvent.LightGuideTrackHit == 1","colz");'

  etc.

The file also includes a number of fixed histograms, which includes all of the input files (optical and radiation damage data points) that are used to calculate reflectivities, absorption lengths, and hot position dependent radiation damage. There are also output histograms that are always needed to evaluate the performance of a given geometry, such as the number of photoelectrons at the cathode.

### Comments

The geometry changes that can be controlled from the macro all assume that the light guide is of the basic form of ring 5, with
flat guide reflectors. This should, in principle, work without problems for rings <= 5, but not necessarily for ring 6, which may require a curved guide. For that case, a complete rework of the light guide class may be required.  