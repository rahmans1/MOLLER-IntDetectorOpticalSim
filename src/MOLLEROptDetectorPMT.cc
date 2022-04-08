#include "MOLLEROptDetectorPMT.hh"

MOLLEROptDetectorPMT::MOLLEROptDetectorPMT(MOLLEROptTrackingReadout *TrRO, G4String name, MOLLEROptMaterial* mat, MOLLEROptDetectorLightGuide* guide)
{
  Name = name+"_PMT";

  LightGuide = guide;
  TrackingReadout = TrRO;
  Materials = mat;
  PMTMaterial = Materials->GetMaterial("Air");  //PMT with quartz window
  PMTCathodeMaterial = Materials->GetMaterial("Aluminum");//Photocathode");  //PMT with quartz window
  PMTWindowMaterial = Materials->GetMaterial("Quartz");  //PMT with quartz window

  PMTLogical  = NULL;
  PMTPhysical = NULL;
  Rotation    = NULL;
  CathSD      = NULL;

  PMTCathodeSurface = NULL;
  CathodeMatPropTable = NULL;
  PMTCathodeBorderSurface = NULL;
  PMTWindowLogicalSkinSurface = NULL;

  CathodeRadius = 4.5*cm;
  CathodeThickness = 0.1*cm;
  PMTThickness = 5.0*cm;
  PMTWindowThickness = 0.1*cm;
  
  PositionX = 0.0*cm;
  PositionY = 0.0*cm; 
  PositionZ = 0.0*cm; 
  // PositionCathodeX = 0.0*cm;
  // PositionCathodeY = 0.0*cm; 
  // PositionCathodeZ = 0.0*cm;
  PositionCathodeX = PositionX;
  PositionCathodeY = PositionY;
  PositionCathodeZ = PMTThickness/2.0 - CathodeThickness/2.0 -  PMTWindowThickness;

  PositionWindowX = PositionX;
  PositionWindowY = PositionY;
  PositionWindowZ = PMTThickness/2.0 - PMTWindowThickness/2.0; 

}

MOLLEROptDetectorPMT::~MOLLEROptDetectorPMT()
{

}

void MOLLEROptDetectorPMT::Initialize()
{
  PMTSolid = new G4Tubs(Name+"_Solid",0,CathodeRadius+0.4*cm,PMTThickness/2,0,2.0*TMath::Pi());
  PMTLogical = new G4LogicalVolume(PMTSolid,PMTMaterial,Name+"_Logical");
  //CreateOpticalSurface(PMTLogical);
  G4Colour  grey      ( 127/255., 127/255., 127/255.);
  G4VisAttributes* VisAtt = new G4VisAttributes(grey);
  VisAtt->SetVisibility(true);
  VisAtt->SetForceWireframe(true);
  PMTLogical->SetVisAttributes(VisAtt);

  PMTWindowSolid = new G4Tubs(Name+"_Window_Solid",0,CathodeRadius+0.4*cm,PMTWindowThickness/2,0,2.0*TMath::Pi());
  PMTWindowLogical = new G4LogicalVolume(PMTSolid,PMTWindowMaterial,Name+"_Window_Logical");
  //CreateOpticalSurface(PMTWindowLogical);
  // G4MaterialPropertiesTable *WindowMatPropTable = new G4MaterialPropertiesTable();
  // WindowMatPropTable = new G4MaterialPropertiesTable();
  // WindowMatPropTable->AddProperty ( "REFLECTIVITY", Optpar->EPhoton,Optpar->QRefl, Optpar->GetNPar());  
  // PMTWindowSurface->SetMaterialPropertiesTable ( WindowMatPropTable );
  G4Colour yellow   ( 255/255., 255/255.,   0/255.);
  VisAtt = new G4VisAttributes(yellow);
  VisAtt->SetVisibility(true);
  VisAtt->SetForceWireframe(true);
  PMTWindowLogical->SetVisAttributes(VisAtt);

  PMTCathodeSolid = new G4Tubs(Name+"_Cathode_Solid",0,CathodeRadius,CathodeThickness/2,0,2.0*TMath::Pi());  
  PMTCathodeLogical = new G4LogicalVolume(PMTCathodeSolid,PMTCathodeMaterial,Name+"_Cathode_Logical");
  CreateOpticalSurface(PMTCathodeLogical);
  //G4UserLimits *limits = new G4UserLimits();
  //limits->SetMaxAllowedStep(0.05*cm);
  //PMTCathodeLogical->SetUserLimits(limits);
  G4Colour  orange    ( 255/255., 127/255.,   0/255.);
  G4VisAttributes* VisAtt2 = new G4VisAttributes(orange);
  VisAtt2->SetVisibility(true);
  VisAtt2->SetForceWireframe(true);
  PMTCathodeLogical->SetVisAttributes(VisAtt2);

  SDman = G4SDManager::GetSDMpointer();
  CathSD = new MOLLEROptPMTSD("/Cathode",TrackingReadout);
  SDman->AddNewDetector(CathSD);  
  PMTWindowLogical->SetSensitiveDetector(CathSD);

}

void MOLLEROptDetectorPMT::CreateOpticalSurface(G4LogicalVolume *logV)
{
  OptParam* Optpar = Materials->GetOpticalParametersTable();

  PMTCathodeSurface = new G4OpticalSurface (Name+"CathodeSurface", glisur , polished, dielectric_metal);
  // PMTCathodeSurface->SetPolish ( 0.99 );
  // PMTCathodeSurface->SetFinish ( polishedbackpainted );

  CathodeMatPropTable = new G4MaterialPropertiesTable();
  
  // CathodeMatPropTable->AddProperty ( "TRANSMITTANCE", Optpar->EPhoton, Optpar->QTrans, Optpar->GetNPar());  
  //CathodeMatPropTable->AddProperty ( "REFLECTIVITY", Optpar->EPhoton, Optpar->QRefl, Optpar->GetNPar());  
  CathodeMatPropTable->AddProperty ( "EFFICIENCY", Optpar->EPhoton, Optpar->QEff, Optpar->GetNPar());  
  CathodeMatPropTable->AddProperty ( "REALRINDEX", Optpar->EPhoton, Optpar->Cath_RIndR, Optpar->GetNPar());  
  CathodeMatPropTable->AddProperty ( "IMAGINARYRINDEX", Optpar->EPhoton, Optpar->Cath_IIndR, Optpar->GetNPar());  
  PMTCathodeSurface->SetMaterialPropertiesTable ( CathodeMatPropTable );

  PMTWindowLogicalSkinSurface = new G4LogicalSkinSurface(Name+"_SkinSurface",logV,PMTCathodeSurface);

}
// void MOLLEROptDetectorPMT::CreateOpticalSurface(G4VPhysicalVolume *window, G4VPhysicalVolume *cathode)
// {
//   OptParam* Optpar = Materials->GetOpticalParametersTable();

//   if(!PMTCathodeSurface)
//     PMTCathodeSurface = new G4OpticalSurface (Name+"_CathodeSurface",glisur , polished, dielectric_metal );
//   //PMTCathodeSurface->SetPolish ( 0.99 );

//   if(!CathodeMatPropTable){
//     CathodeMatPropTable = new G4MaterialPropertiesTable();
//     CathodeMatPropTable->AddProperty ( "REFLECTIVITY", Optpar->EPhoton, Optpar->LGRefl90, Optpar->GetNPar());  
//     //CathodeMatPropTable->AddProperty ( "TRANSMITTANCE", Optpar->EPhoton, Optpar->QTrans, Optpar->GetNPar());  
//     CathodeMatPropTable->AddProperty ( "Efficiency", Optpar->EPhoton, Optpar->QEff, Optpar->GetNPar());  
//     PMTCathodeSurface->SetMaterialPropertiesTable ( CathodeMatPropTable );
//   }

//   if(!PMTCathodeBorderSurface)
//     PMTCathodeBorderSurface = new G4LogicalBorderSurface(Name+"_CathodeBorderSurface",window,cathode,PMTCathodeSurface);

// }

G4VPhysicalVolume* MOLLEROptDetectorPMT::Construct(G4VPhysicalVolume* Mother)
{

  cout << "In MOLLEROptDetectorPMT::Construct" << endl;

  MotherVolume = Mother;
  
  Rotation = new G4RotationMatrix;
  Rotation->rotateX(-TMath::Pi()/2.0*rad);
  
  Position  = G4ThreeVector(PositionX,
			    PositionY,
			    PositionZ);

  cout << endl;
  cout << endl;
  cout << PositionX << " " << PositionY << " " << PositionZ << endl;
  cout << endl;
  cout << endl;
  
  PMTPhysical = new G4PVPlacement(Rotation,  
				  Position, 
				  Name+"_Physical", 
				  PMTLogical,
				  MotherVolume, 
				  false, 
				  4);
  // PositionCathodeX = PositionX;
  // PositionCathodeY = PositionY;
  // PositionCathodeZ = PositionZ + PMTThickness/2 - PMTWindowThickness;
  
  
  Position  = G4ThreeVector(PositionWindowX,
			    PositionWindowY,
			    PositionWindowZ);
  // G4RotationMatrix* RotationW = new G4RotationMatrix;  
  // RotationW->rotateX(TMath::Pi()*rad);

  
  PMTWindowPhysical = new G4PVPlacement(0,  
					Position, 
					Name+"_Window_Physical", 
					PMTWindowLogical,
					PMTPhysical, 
					false, 
					5);
  
  Position  = G4ThreeVector(PositionCathodeX,
			    PositionCathodeY,
			    PositionCathodeZ);
  
  PMTCathodePhysical = new G4PVPlacement(0,  
					 Position, 
					 Name+"_Cathode_Physical", 
					 PMTCathodeLogical,
					 PMTPhysical, 
					 false, 
					 6);



  // CreateOpticalSurface(PMTWindowPhysical,PMTCathodePhysical);
  
  return PMTPhysical;
} 

void MOLLEROptDetectorPMT::SetMaterial(G4String materialName)
{

}


void MOLLEROptDetectorPMT::SetCenterPositionInX(G4double xPos)
{
    PositionX = xPos;	 

    if(PMTPhysical)
      PMTPhysical->SetTranslation(G4ThreeVector(PositionX,
						PositionY, 
						PositionZ));
    
}

void MOLLEROptDetectorPMT::SetCenterPositionInY(G4double yPos)
{
    PositionY = yPos;
    //PositionCathodeZ = PMTWindowThickness;

    if(PMTPhysical)
      PMTPhysical->SetTranslation(G4ThreeVector(PositionX,
						PositionY, 
						PositionZ));

    // //this is cathode local z
    // PositionCathodeZ = yPos - PMTWindowThickness;	 
    
    // if(PMTCathodePhysical)
    //   PMTCathodePhysical->SetTranslation(G4ThreeVector(PositionCathodeX,
    // 						       PositionCathodeY, 
    // 						       PositionCathodeZ));

}

void MOLLEROptDetectorPMT::SetCenterPositionInZ(G4double zPos)
{
    PositionZ = zPos;

    if(PMTPhysical)
      PMTPhysical->SetTranslation(G4ThreeVector(PositionX,
						PositionY, 
						PositionZ));
}

void MOLLEROptDetectorPMT::ClearVolumes()
{
  //SDman->Activate("/Cathode",false);
  if(CathSD)      delete CathSD;
  if(PMTPhysical) delete PMTPhysical;
  if(PMTLogical)  delete PMTLogical;
  if(PMTCathodePhysical) delete PMTCathodePhysical;
  if(PMTCathodeLogical)  delete PMTCathodeLogical;
}


void MOLLEROptDetectorPMT::UpdateGeometry()
{  
  G4SolidStore::GetInstance()->DeRegister(PMTCathodeSolid);
  G4SolidStore::GetInstance()->DeRegister(PMTWindowSolid);
  G4SolidStore::GetInstance()->DeRegister(PMTSolid);

  delete PMTCathodeSolid;
  delete PMTWindowSolid;
  delete PMTSolid;

  delete PMTPhysical;
  delete PMTCathodePhysical;
  delete PMTWindowPhysical;
  
  PMTSolid = new G4Tubs(Name+"_Solid",0,CathodeRadius+0.4*cm,PMTThickness/2,0,2.0*TMath::Pi());
  PMTCathodeSolid = new G4Tubs(Name+"_Cathode_Solid",0,CathodeRadius,CathodeThickness/2,0,2.0*TMath::Pi());
  PMTWindowSolid = new G4Tubs(Name+"_Window_Solid",0,CathodeRadius,PMTWindowThickness/2,0,2.0*TMath::Pi());


  PMTLogical->SetSolid(PMTSolid);  
  PMTCathodeLogical->SetSolid(PMTCathodeSolid);  
  PMTWindowLogical->SetSolid(PMTWindowSolid);  
}


