#ifndef MOLLEROptDetectorQuartz_h
#define MOLLEROptDetectorQuartz_h 

#include "cpp_include.h"
#include "Root_include.h"
#include "Geant4_include.hh"
#include "G4VisExtent.hh"


#include "MOLLEROptQuartzSD.hh"
#include "MOLLEROptMaterial.hh"
#include "MOLLEROptTrackingReadout.hh"

class MOLLEROptDetectorQuartz 
{
public:
  MOLLEROptDetectorQuartz(MOLLEROptTrackingReadout*, G4String type, MOLLEROptMaterial* mat);
  ~MOLLEROptDetectorQuartz();

  void Construct(G4VPhysicalVolume* Mother);
  void Initialize();
  void SetMaterial(G4String materialName);

  void SetCenterPositionInX(G4double xPos);
  void SetCenterPositionInY(G4double yPos);
  void SetCenterPositionInZ(G4double zPos);
  G4double GetCurrentCenterPositionInX() {return QuartzPhysical->GetTranslation().x();}//PositionX;};
  G4double GetCurrentCenterPositionInY() {return QuartzPhysical->GetTranslation().y();}//PositionY;};
  G4double GetCurrentCenterPositionInZ() {return QuartzPhysical->GetTranslation().z();}//PositionZ;};
  G4ThreeVector GetCurrentCenterPosition() {return QuartzPhysical->GetTranslation();};
  void SetAzimuthalRotationAngle(G4double a); //w.r.t. to the positive y axis (up) in radians
  void SetPolarRotationAngle(G4double a); //w.r.t. to the positive z axis (downstream) in radians
  
  //The standard coordinate system is: z=beam direction, y=beam up, x = beam left
  //Dimensions below assume a detector that is located at the origin with the 45 degreee
  //exit face at the upper y end and the width of the detector along +-x.
  void SetQuartzSizeX(G4double x){FullLengthX = x;}; 
  void SetQuartzSizeY(G4double y){FullLengthY = y;}; 
  void SetQuartzSizeZ(G4double z){FullLengthZ = z;};
  void SetBevelSize(G4double bev) {bevel = bev;};
  void SetQuartzRotX(G4double r);
  G4double GetQuartzSizeX(){return FullLengthX;}; 
  G4double GetQuartzSizeY(){return FullLengthY;}; 
  G4double GetQuartzSizeZ(){return FullLengthZ;};

  G4double GetQuartzRotationX(){return QRotationX;};

  void GetQuartzLimits(G4double *vals);

  void UpdateGeometry();
  void ClearVolumes();
  void SetSegRadDamageFlag() {SegRadDamageFlag = true;};

  G4LogicalVolume*   GetLogicalVolume()    {return QuartzLogical;} 
  G4VPhysicalVolume* GetPhysicalVolume()   {return QuartzPhysical;} 

private:

  G4String Name;
  
  MOLLEROptTrackingReadout *TrackingReadout;
  MOLLEROptMaterial* Materials;
  
  // MOLLEROptDetectorMessenger* DetMessenger;

  G4VPhysicalVolume* MotherVolume;

  G4LogicalVolume*   QuartzLogical; 
  G4VPhysicalVolume* QuartzPhysical; 
  G4Material*        QuartzMaterial;
  G4SubtractionSolid* QuartzSolid;
  //G4GenericTrap*     QuartzSolid;

  G4double FullLengthX;
  G4double FullLengthY;
  G4double FullLengthZ;

  std::vector<G4TwoVector> Vertices;

  
  G4ThreeVector     Position;
  G4RotationMatrix* Rotation;
  G4double   QRotationX;

  G4double PositionX;
  G4double PositionY;
  G4double PositionZ;

  G4SDManager* SDman;
  G4VSensitiveDetector* QuartzSD;

  G4bool SegRadDamageFlag;
  
  OptParam* OpticalParameters;

  double bevel;
};
#endif
