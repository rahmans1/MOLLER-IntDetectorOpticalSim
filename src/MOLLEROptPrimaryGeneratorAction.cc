#include "MOLLEROptPrimaryGeneratorAction.hh"

MOLLEROptPrimaryGeneratorAction::MOLLEROptPrimaryGeneratorAction(MOLLEROptConstruction* Constr)
{

  Messenger = new MOLLEROptPrimaryGeneratorActionMessenger(this);

  EventCounter = 0;
  EventRegion = 1; 
  EventRegionOffsetX = 0;
  EventRegionOffsetY = 0;

  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);

  Construction = Constr;

  G4ParticleDefinition* particle = G4Electron::Definition();
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleEnergy(8000.0*MeV);
}

MOLLEROptPrimaryGeneratorAction::~MOLLEROptPrimaryGeneratorAction()
{
  if (particleGun) delete particleGun;
}

void MOLLEROptPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4double x = 0;
  G4double y = 0;
    
  G4double Qlim[4];
  G4double LGlim[8];
  Construction->GetQuartzLimits(Qlim);
  Construction->GetLightGuideLimits(LGlim);

  if(Qlim[0]<=0) Qlim[0] += 1; else Qlim[0] -= 1; 
  if(Qlim[1]<=0) Qlim[1] += 1; else Qlim[1] -= 1; 
  if(Qlim[2]<=0) Qlim[2] += 1; else Qlim[2] -= 1; 
  if(Qlim[3]<=0) Qlim[3] += 1; else Qlim[3] -= 1; 

  if(LGlim[0]<=0) LGlim[0] += 1; else LGlim[0] -= 1; 
  if(LGlim[1]<=0) LGlim[1] += 1; else LGlim[1] -= 1; 
  if(LGlim[2]<=0) LGlim[2] += 1; else LGlim[2] -= 1; 
  if(LGlim[3]<=0) LGlim[3] += 1; else LGlim[3] -= 1; 
  if(LGlim[4]<=0) LGlim[4] += 1; else LGlim[4] -= 1; 
  if(LGlim[5]<=0) LGlim[5] += 1; else LGlim[5] -= 1; 
  if(LGlim[6]<=0) LGlim[6] += 1; else LGlim[6] -= 1; 
  if(LGlim[7]<=0) LGlim[7] += 1; else LGlim[7] -= 1; 

  // //On Quartz    
  // G4double x = -4.20 + 8.40*G4UniformRand();//-3.8 + 7.6*G4UniformRand();  //-8.5 + 17.0*G4UniformRand();
  // G4double y = -7.5 + 15.0*G4UniformRand();//-6.7 + 13.4*G4UniformRand(); //-2.5 + 5.0*G4UniformRand();
  // //On Lower Light Guide
  // G4double x = -3.8 + 7.6*G4UniformRand();//-42.0 + 84.0*G4UniformRand();  //-87.5 + 175.0*G4UniformRand();
  // G4double y = 8.0 + 6.0*G4UniformRand();//-72.5 + 145.0*G4UniformRand(); //-25.0 + 50.0*G4UniformRand();
  // //On Upper Light Guide
  // G4double x = -3.8 + 7.6*G4UniformRand();-42.0 + 84.0*G4UniformRand();  //-87.5 + 175.0*G4UniformRand();
  // // G4double y = 13.0 + 19.0*G4UniformRand();//-72.5 + 145.0*G4UniformRand(); //-25.0 + 50.0*G4UniformRand();
  // G4double y = 14.0 + 24.0*G4UniformRand();-72.5 + 145.0*G4UniformRand(); //-25.0 + 50.0*G4UniformRand();

  if(EventRegion == 2){
  //On Lower Light Guide cone
    x = LGlim[0] + (LGlim[1]-LGlim[0])*G4UniformRand();
    y = LGlim[2] + (LGlim[3]-LGlim[2])*G4UniformRand();
    
  }
  else if(EventRegion == 3){
    //On Upper Light Guide cone
    x = LGlim[4] + (LGlim[5]-LGlim[4])*G4UniformRand();
    y = LGlim[6]+10*mm + (LGlim[7]-LGlim[6]-20*mm)*G4UniformRand();
  }
  else if(EventRegion == 4){
    //2x2 mm^2 spot on quartz
    x = (Qlim[1]+Qlim[0])/2.0 -2 +4*G4UniformRand()+EventRegionOffsetX;
    y = (Qlim[3]+Qlim[2])/2.0 -2 +4*G4UniformRand()+EventRegionOffsetY;
  }
  else if(EventRegion == 5){
    //2x2 mm^2 spot on lower guide cone
    x = (LGlim[1]+LGlim[0])/2.0 -2 +4*G4UniformRand()+EventRegionOffsetX;
    y = (LGlim[3]+LGlim[2])/2.0 -2 +4*G4UniformRand()+EventRegionOffsetY;
  }  
  else if(EventRegion == 6){
    //2x2 mm^2 spot on upper guide cone
    x = (LGlim[5]+LGlim[4])/2.0 -2 +4*G4UniformRand()+EventRegionOffsetX;
    y = (LGlim[7]+LGlim[6])/2.0 -2 +4*G4UniformRand()+EventRegionOffsetY;
  }  
  else{ 
    //On Quartz
    x = Qlim[0] + (Qlim[1]-Qlim[0])*G4UniformRand();
    y = Qlim[2] + (Qlim[3]-Qlim[2])*G4UniformRand();
  }


  particleGun->SetParticlePosition(G4ThreeVector(x*mm,y*mm, -300.0*mm));
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

  particleGun->GeneratePrimaryVertex(anEvent);
  EventCounter += 1;

  // myUserInfo->SetPrimaryEventNumber( (G4int) myEventCounter );
  
  if(EventCounter%100 == 0)
    G4cout << "Capture Event# " << EventCounter << G4endl;
}
