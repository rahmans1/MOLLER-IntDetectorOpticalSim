//=============================================================================
// 
//   ---------------------------
//  | Doxygen File Information |
//  ---------------------------
/**
 
   \file Geant4_include.hh
   $Revision: 1.2 $	
   $Date: 2005/12/27 19:20:25 $
   \author Klaus Hans Grimm
     
*/
//=============================================================================
//
//=============================================================================
//   -----------------------
//  | CVS File Information |
//  -----------------------
// 
//  Last Update:      $Author: grimm $
//  Update Date:      $Date: 2005/12/27 19:20:25 $
//  CVS/RCS Revision: $Revision: 1.2 $
//  Status:           $State: Exp $
// 
// ===================================
//  CVS Revision Log at end of file !!
// ===================================
// 
//=============================================================================


//////////////////////////////////////////////////////////
// header Geant4_include.h
//////////////////////////////////////////////////////////

#ifndef __Geant4_include_h
#define __Geant4_include_h

//using namespace std;

//================================================

//================================================



// Run related
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UserRunAction.hh"

// material related
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"


// touchable related
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"

// step/track related

#include "G4Step.hh"
#include "G4StepLimiter.hh"
#include "G4StepPoint.hh"
#include "G4SteppingManager.hh"
#include "G4UserSteppingAction.hh"
#include "G4SteppingManager.hh"
#include "G4SteppingVerbose.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4UserTrackingAction.hh"
#include "G4VUserTrackInformation.hh"
#include "G4TrackingManager.hh"
#include "G4TrackVector.hh"

// trajectory related
#include "G4Trajectory.hh"
#include "G4VTrajectory.hh"
#include "G4TrajectoryPoint.hh"


// HEP related 
#include "globals.hh"
#include "Randomize.hh"

// material related
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"

// relocation related
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"


// geometry related
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4GenericTrap.hh"
#include "G4Trap.hh"
#include "G4Cons.hh"
#include "G4Para.hh"
#include "G4Polyline.hh"
#include "G4Polymarker.hh"
#include "G4Circle.hh"
#include "G4Polyhedra.hh"

// manager related
#include "G4GeometryManager.hh"
#include "G4SDManager.hh"
#include "G4VVisManager.hh"
#include "G4ProcessManager.hh"
#include "G4Navigator.hh"

// event related
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4UserEventAction.hh"

#include "G4UserLimits.hh"

// attributes
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"

// store related
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4RegionStore.hh"
#include "G4SolidStore.hh"

// detector related
#include "G4VUserDetectorConstruction.hh"
#include "G4VSensitiveDetector.hh"
#include "G4VReadOutGeometry.hh"


// volume related
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
//#include "G4AssembyVolume.hh"


// optical related
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"


#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4VPVParameterisation.hh"
#include "G4PVReplica.hh"


// field related
#include "G4MagneticField.hh"

#include "G4UniformMagField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"
#include "G4PropagatorInField.hh"


#include "G4ExplicitEuler.hh"
#include "G4ImplicitEuler.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4ClassicalRK4.hh"
#include "G4HelixExplicitEuler.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4HelixSimpleRunge.hh"
#include "G4CashKarpRKF45.hh"
#include "G4RKG3_Stepper.hh"


// boolean 
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"

// particle related
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleWithCuts.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"


// hits related
#include "G4HCofThisEvent.hh"
#include "G4THitsCollection.hh"
#include "G4VHitsCollection.hh"
#include "G4VHit.hh"

// trajectory related
#include "G4Trajectory.hh"
#include "G4TrajectoryContainer.hh"
#include "G4TrajectoryPoint.hh"


// physics related
// #include "G4VUserPhysicsList.hh"
// #include "G4ProcessManager.hh"
// #include "G4ParticleTypes.hh"
// #include "G4ComptonScattering.hh"
// #include "G4GammaConversion.hh"
// #include "G4PhotoElectricEffect.hh"
// #include "G4MultipleScattering.hh"
// #include "G4eIonisation.hh"
// #include "G4ionIonisation.hh"
// #include "G4eBremsstrahlung.hh"
// #include "G4eplusAnnihilation.hh"
// #include "G4MuIonisation.hh"
// #include "G4MuBremsstrahlung.hh"
// #include "G4MuPairProduction.hh"
// #include "G4hIonisation.hh"
// #include "G4UserSpecialCuts.hh"
// #include "G4CascadeInterface.hh"
// #include "G4NeutronIsotopeProduction.hh"

// #include "G4Decay.hh"
// #include "G4RadioactiveDecay.hh"
// #include "G4IonTable.hh"
// #include "G4Ions.hh"






// //********************************************
// //Added for low energy hadronic tests 04/27/06
// //but may not be necessary or work well

// // gamma
// #include "G4LowEnergyRayleigh.hh" 
// #include "G4LowEnergyPhotoElectric.hh"
// #include "G4LowEnergyCompton.hh"  
// #include "G4LowEnergyGammaConversion.hh" 

// // e-
// #include "G4LowEnergyIonisation.hh" 
// #include "G4LowEnergyBremsstrahlung.hh" 

// // alpha and GenericIon and deuterons, triton, He3:
// #include "G4hLowEnergyIonisation.hh"
// #include "G4EnergyLossTables.hh"
// // hLowEnergyIonisation uses Ziegler 1988 as the default

// #include "G4MuonMinusCaptureAtRest.hh"


// // Elastic processes:
// #include "G4HadronElasticProcess.hh"

// // Inelastic processes:
// #include "G4PionPlusInelasticProcess.hh"
// #include "G4PionMinusInelasticProcess.hh"
// #include "G4KaonPlusInelasticProcess.hh"
// #include "G4KaonZeroSInelasticProcess.hh"
// #include "G4KaonZeroLInelasticProcess.hh"
// #include "G4KaonMinusInelasticProcess.hh"
// #include "G4ProtonInelasticProcess.hh"
// #include "G4AntiProtonInelasticProcess.hh"
// #include "G4NeutronInelasticProcess.hh"
// #include "G4AntiNeutronInelasticProcess.hh"
// #include "G4DeuteronInelasticProcess.hh"
// #include "G4TritonInelasticProcess.hh"
// #include "G4AlphaInelasticProcess.hh"

// // Low-energy Models: < 20GeV
// #include "G4LElastic.hh"
// #include "G4LEPionPlusInelastic.hh"
// #include "G4LEPionMinusInelastic.hh"
// #include "G4LEKaonPlusInelastic.hh"
// #include "G4LEKaonZeroSInelastic.hh"
// #include "G4LEKaonZeroLInelastic.hh"
// #include "G4LEKaonMinusInelastic.hh"
// #include "G4LEProtonInelastic.hh"
// #include "G4LEAntiProtonInelastic.hh"
// #include "G4LENeutronInelastic.hh"
// #include "G4LEAntiNeutronInelastic.hh"
// #include "G4LEDeuteronInelastic.hh"
// #include "G4LETritonInelastic.hh"
// #include "G4LEAlphaInelastic.hh"

// // High-energy Models: >20 GeV
// #include "G4HEPionPlusInelastic.hh"
// #include "G4HEPionMinusInelastic.hh"
// #include "G4HEKaonPlusInelastic.hh"
// #include "G4HEKaonZeroInelastic.hh"
// #include "G4HEKaonZeroInelastic.hh"
// #include "G4HEKaonMinusInelastic.hh"
// #include "G4HEProtonInelastic.hh"
// #include "G4HEAntiProtonInelastic.hh"
// #include "G4HENeutronInelastic.hh"
// #include "G4HEAntiNeutronInelastic.hh"

// // Neutron high-precision models: <20 MeV
// #include "G4NeutronHPElastic.hh"
// #include "G4NeutronHPElasticData.hh"
// #include "G4NeutronHPCapture.hh"
// #include "G4NeutronHPCaptureData.hh"
// #include "G4NeutronHPInelastic.hh"
// #include "G4NeutronHPInelasticData.hh"
// #include "G4LCapture.hh"

// // Stopping processes
// #include "G4PiMinusAbsorptionAtRest.hh"
// #include "G4KaonMinusAbsorptionAtRest.hh"
// #include "G4AntiProtonAnnihilationAtRest.hh"
// #include "G4AntiNeutronAnnihilationAtRest.hh"

// //********************************************






//misc
#include "G4Allocator.hh"

// GUI related
#include "G4UImanager.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"


#endif

//=======================================================================================
//   -----------------------
//  | CVS File Information |
//  -----------------------
// 
//      $Revisions$  
//      $Log: Geant4_include.hh,v $
//      Revision 1.2  2005/12/27 19:20:25  grimm
//      - Redesign of Doxygen header containing CVS info like revision and date
//      - Added CVS revision log at the end of file
//
// 
