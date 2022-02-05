#include "MOLLEROptMaterial.hh"

MOLLEROptMaterial::MOLLEROptMaterial(MOLLEROptTrackingReadout* trRO)
{
  TrackingReadout = trRO;
  NOptPar = 58;
  OptPar = new OptParam(NOptPar);
  SetOpticalParameters();
  SegRadDamgeFlag = false;  
}

MOLLEROptMaterial::~MOLLEROptMaterial()
{

}

void MOLLEROptMaterial::SetSegRadDamage()
{
  //   OptParam *OptPar = Materials->GetOpticalParametersTable();
  // G4int nEntries = OptPar->GetNPar();
  
  
  
}

void MOLLEROptMaterial::CalculateAbsorptionLengths(G4Element* elSi, G4Element* elO)
{
  Float_t x,y, tmp, tmp2;
  Int_t n = 0, bin;
  Double_t eng, raddam;
  Double_t *absvec = new Double_t[NOptPar]; 
  Double_t trns, abs;

  G4String name;        
  G4double density;     
  G4int    ncomponents; 
  G4Material* mat;      
   
  TH1D *data35MRad_before   = new TH1D("data35MRad_before","",600,200,800);
  TH1D *data35MRad_after   = new TH1D("data35MRad_after","",600,200,800);
  TH1D *data170MRad_after  = new TH1D("data170MRad_after","",600,200,800);
  TH1D *tmpRefl  = new TH1D("tmpRefl","",600,200,800);

  TProfile *Trans35 = new TProfile("Trans35","",NOptPar,1.51,6.25,0,110.0); 
  TProfile *Trans70 = new TProfile("Trans70","",NOptPar,1.51,6.25,0,110.0); 
  TProfile *Trans170 = new TProfile("Trans170","",NOptPar,1.51,6.25,0,110.0);
  //vector <TProfile*> TransX;
  TProfile *Reflectance = new TProfile("Reflectance","",NOptPar,1.51,6.25,0,10.0); 

  ifstream iFileReflectance("../data/FusedSilicaReflectance.txt");
  while(!iFileReflectance.eof()){
    
    iFileReflectance >> x >> y;
    bin = tmpRefl->FindBin(round(x));
    tmpRefl->SetBinContent(bin,y);
  }
  iFileReflectance.close();
  for(int p = 402; p <= 600; p++){
    tmpRefl->SetBinContent(p,7.9361-0.00328023*tmpRefl->GetBinCenter(p)+2.10127e-6*tmpRefl->GetBinCenter(p)*tmpRefl->GetBinCenter(p));
  }

  for(int b = 1; b <= 600; b++){

    eng = 1242.0/tmpRefl->GetBinCenter(b);
    Reflectance->Fill(eng,tmpRefl->GetBinContent(b),1);
  }
  
  ifstream iFile35MRadBefore("../data/SP3C.ASC");
    while(!iFile35MRadBefore.eof()){
    iFile35MRadBefore >> x >> y;
    bin = data35MRad_before->FindBin(x);
    data35MRad_before->SetBinContent(bin,y);
  }
  iFile35MRadBefore.close();
    
  ifstream iFile35MRadAfter("../data/SP3AA.ASC");
    while(!iFile35MRadAfter.eof()){
    iFile35MRadAfter >> x >> y;
    eng = 1242.0/x;
    Trans35->Fill(eng,y,1);
    bin = data35MRad_after->FindBin(x);
    data35MRad_after->SetBinContent(bin,y);
  }
  iFile35MRadAfter.close();

  
  ifstream iFile170MRadAfter("../data/SP3CA.ASC");
  while(!iFile170MRadAfter.eof()){

    iFile170MRadAfter >> x >> y;
    eng = 1242.0/x;
    Trans170->Fill(eng,y,1);
    bin = data170MRad_after->FindBin(x);
    data170MRad_after->SetBinContent(bin,y);
    tmp = data35MRad_before->GetBinContent(bin);
    tmp2 = tmp + 70*(y - tmp)/170; //this is 70 MRad Damage    
    Trans70->Fill(eng,tmp2,1);    
  }
  iFile170MRadAfter.close();

  TrackingReadout->StoreQuartzOpticalPlots(Trans35,Trans70,Trans170,Reflectance,QEffhst);
  
  for(int bx = 1; bx <= PixellatedRelativeRate->GetNbinsX(); bx++){
    for(int by = 1; by <= PixellatedRelativeRate->GetNbinsY(); by++){

      TProfile *QAbsorption  = new TProfile(Form("QAbsorption_%1.2f_%1.2f_%1.2f",PixellatedRelativeRate->GetBinContent(bx,by),PixellatedRelativeRate->GetXaxis()->GetBinCenter(bx),PixellatedRelativeRate->GetYaxis()->GetBinCenter(by)),"",NOptPar,1.51,6.21,0,250.0);
      
      for(int bin = 1; bin <= 600; bin++){
	eng = 1242.0/data35MRad_before->GetXaxis()->GetBinCenter(bin);
	raddam = PixellatedRelativeRate->GetBinContent(bx,by);
	trns = data35MRad_before->GetBinContent(bin) + raddam*70*(data170MRad_after->GetBinContent(bin) - data35MRad_before->GetBinContent(bin))/170;
	abs = -1.0/TMath::Log(trns/100.0/(1.0 - 0.01*tmpRefl->GetBinContent(bin)));
	QAbsorption->Fill(eng,abs,1);
	
      }
      for(int v = 0; v < NOptPar; v++){
      	eng = OptPar->EPhoton[v]/eV;
      	absvec[v] = QAbsorption->GetBinContent(QAbsorption->FindBin(eng))*cm;
      	//cout  << "Eng = " << eng << " Abs = "  << absvec[v] << " opt70 = " << OptPar->QAbs70[v] << endl;
      }

      name        = Form("QuartzCell_%d_%d",bx,by);
      density     = 2.203*g/cm3;
      ncomponents = 2;
      mat         = new G4Material(name,density,ncomponents);
      mat->AddElement(elSi, 1);
      mat->AddElement(elO , 2);
            
      G4MaterialPropertiesTable *mpt = new G4MaterialPropertiesTable();
      mpt->AddProperty("RINDEX",    OptPar->EPhoton, RefractiveIndex_Quartz , NOptPar);
      mpt->AddProperty("ABSLENGTH", OptPar->EPhoton, absvec, NOptPar);
      mat->SetMaterialPropertiesTable(mpt);
      
      QuartzCells_MPT.push_back(mpt);
      
      OptPar->AddNewQAbs(PixellatedRelativeRate->GetXaxis()->GetBinCenter(bx),
      			 PixellatedRelativeRate->GetYaxis()->GetBinCenter(by),
      			 PixellatedRelativeRate->GetXaxis()->GetBinWidth(bx)/2,
      			 PixellatedRelativeRate->GetYaxis()->GetBinWidth(by)/2,
			 mat);
      
      TrackingReadout->SetNewQAbsorptionProfile(QAbsorption);
      
      n++;
    }    
  }


  TProfile *QAbsorption  = new TProfile("QAbsorption_NoDam","",NOptPar,1.51,6.21,0,250.0);
      
  for(int bin = 1; bin <= 600; bin++){
    eng = 1242.0/data35MRad_before->GetXaxis()->GetBinCenter(bin);
    trns = data35MRad_before->GetBinContent(bin);
    abs = -1.0/TMath::Log(trns/100.0/(1.0 - 0.01*tmpRefl->GetBinContent(bin)));
    QAbsorption->Fill(eng,abs,1);
  }
  TrackingReadout->SetNewQAbsorptionProfile(QAbsorption);
  
  delete data35MRad_before;
  delete data35MRad_after ;
  delete data170MRad_after;
  delete tmpRefl;

  delete Trans35 ; 
  delete Trans70 ; 
  delete Trans170;
  delete Reflectance; 
}

G4bool MOLLEROptMaterial::GetPixellatedRelativeRate()
{

  TFile *file = new TFile("../data/output2.root","READ");
  if(!file || !file->IsOpen()){
    cout << "Cannot open file: ../data/output2.root needed to read in the pixellated rate on the detector." << endl;
    cout << "Do nothing here ..." << endl;
    return 0;
  }
  
  Double_t pi = TMath::Pi();
  Double_t minX = 0, maxX = 0, minY = 0, maxY = 0;
  Int_t cbinX, zbinX;
  Int_t cbinY, zbinY;
  Double_t maxVal = 0;
  TH2D *hst = 0;
  
  hst = (TH2D*)gDirectory->Get("h");
    
  if(hst){
      
    int NBinsX = hst->GetXaxis()->GetNbins();
    int NBinsY = hst->GetYaxis()->GetNbins();
    
    TH2D *hst2 = (TH2D*)hst->Clone();
    hst2->Reset();
    hst2->SetName(Form("%s_cp","h"));
      
    Double_t xV; 
    Double_t yV;    
    Double_t bWidth;
      
    Int_t k = 0;
      
    for(int nx = 1; nx < NBinsX; nx++){
      for(int ny = 1; ny < NBinsY; ny++){
	
	if(hst->GetBinContent(nx,ny)){
	    
	  if(fabs(hst->GetXaxis()->GetBinLowEdge(nx)) > 920 && fabs(hst->GetXaxis()->GetBinUpEdge(nx)) < 1060
	     && hst->GetYaxis()->GetBinLowEdge(ny) >= -40 && hst->GetYaxis()->GetBinUpEdge(ny) <= 40){
	      
	    xV = hst->GetXaxis()->GetBinCenter(nx);
	    yV = hst->GetYaxis()->GetBinCenter(ny);
	    
	    hst2->Fill(xV,yV,hst->GetBinContent(nx,ny));
	    
	    if(!k) {
	      
	      minX = xV;
	      maxX = xV;
	      
	      minY = yV;
	      maxY = yV;		
	      
	    }
	    else{
	      
	      if(xV > maxX) maxX = xV;
	      if(yV > maxY) maxY = yV;
	      if(xV < minX) minX = xV;
	      if(yV < minY) minY = yV;
	    }
	    k++;
	  }
	}
      }
    }
                  
    cbinX = hst2->GetXaxis()->FindBin(0.5*(maxX+minX));
    cbinY = hst2->GetYaxis()->FindBin(0.5*(maxY+minY));
    zbinX = hst2->GetXaxis()->FindBin(0.);
    zbinY = hst2->GetYaxis()->FindBin(0.);
      
    Double_t angle2 = TMath::ATan2(0.5*(maxY+minY),0.5*(maxX+minX));
    if(angle2 < 0) angle2 = 2.0*pi + angle2;	 
      
    Double_t xVR;
    Double_t yVR;
      
    TH2D *hst3 = (TH2D*)hst2->Clone();
    hst3->Reset();
    hst3->SetName(Form("%s_cp",hst2->GetName()));
    
    PixellatedRelativeRate =  new TH2D(Form("%s_cv",hst->GetName()),"",4,-40,40,7,-70,70); //this splits the detector into 2x2 cm^2 "pixels" 
		    
    for(int nx = 1; nx < NBinsX; nx++){
      for(int ny = 1; ny < NBinsY; ny++){
	if(hst2->GetBinContent(nx,ny)){
	  
	  xV = hst2->GetXaxis()->GetBinCenter(nx-(cbinX-zbinX));
	  yV = hst2->GetYaxis()->GetBinCenter(ny-(cbinY-zbinY));
	  
	  xVR = xV*TMath::Sin(angle2) - yV*TMath::Cos(angle2); 
	  yVR = xV*TMath::Cos(angle2) + yV*TMath::Sin(angle2); 
	  
	  hst3->SetBinContent(hst3->GetXaxis()->FindBin(xVR),hst3->GetYaxis()->FindBin(yVR),hst2->GetBinContent(nx,ny));
	  PixellatedRelativeRate->Fill(xVR,yVR,hst2->GetBinContent(nx,ny));	    
	}
      }
    }

    for(int bx = 1; bx <= 4; bx++){
      for(int by = 1; by <= 7; by++){
	if(PixellatedRelativeRate->GetBinContent(bx,by) > maxVal) maxVal = PixellatedRelativeRate->GetBinContent(bx,by);
      }    
    }
    PixellatedRelativeRate->Scale(1.0/maxVal);
    
    hst->SetDirectory(0);
    hst2->SetDirectory(0);
    hst3->SetDirectory(0);
    
    TrackingReadout->SetRadiationDamageHistograms(hst,hst2,hst3,PixellatedRelativeRate);
    
    delete hst;
    delete hst2;
    delete hst3;
    
  }

  return 1;
}


void MOLLEROptMaterial::SetOpticalParameters()
{
  // OptPar->EPhoton[NOptPar] = {1.55, 1.58, 1.61, 1.64, 1.67, 1.70, 1.73, 1.76, 1.79, 1.82, 1.85,
  // 		     1.88, 1.91, 1.94, 1.97, 2.00, 2.03, 2.07, 2.10, 2.14, 2.18, 2.21,
  // 		     2.25, 2.30, 2.34, 2.38, 2.43, 2.48, 2.53, 2.58, 2.64, 2.70, 2.76,
  // 		     2.82, 2.88, 2.95, 3.02, 3.10, 3.18, 3.26, 3.35, 3.44, 3.54, 3.65,
  // 		     3.76, 3.87, 4.00, 4.13, 4.28, 4.43, 4.59, 4.77, 4.96, 5.17, 5.39,
  // 		     5.64, 5.90, 6.20};
 
  

  ifstream optPropFile("data/OpticalPropertiesInputData.txt");
  if(!optPropFile.is_open()){
    G4cout << "Cannot read optical input file named: OpticalPropertiesInputData.txt " << endl;
    G4cout << "The file must be located in the execution directory." << endl;
    G4cout << "It can be found in the data directory of the and must be copied from there." << endl;
    G4cout << "Using default settings." << endl;
  }
  else{
    string header;
    std::getline(optPropFile,header);
    G4cout << "Reading optical properties file." << endl;
    G4cout << "Header: " << endl;
    G4cout << header << endl;
    
    int n = 0;

    Double_t var[12];

    while(!optPropFile.eof()){

      optPropFile >> var[0]
  		  >> var[1]
  		  >> var[2]
  		  >> var[3]
  		  >> var[4]
  		  >> var[5]
  		  >> var[6]
  		  >> var[7]
  		  >> var[8] 
  		  >> var[9] 
  		  >> var[10] 
  		  >> var[11];


      OptPar->EPhoton[n]  = var[0]*eV;
      OptPar->QEff[n]     = var[1]; 
      OptPar->QRefl[n]    = var[2];
      OptPar->QTrans[n]   = 1.0-var[2];
      OptPar->LGRefl30[n] = var[3]; 
      OptPar->LGRefl45[n] = var[4]; 
      OptPar->LGRefl60[n] = var[5]; 
      OptPar->LGRefl90[n] = var[6]; 
      OptPar->QAbs[n]     = var[7]*cm; 
      OptPar->QAbs70[n]   = var[8]*cm; 
      OptPar->QAbs170[n]  = var[9]*cm;
      OptPar->Cath_RIndR[n]  = var[10];
      OptPar->Cath_IIndR[n]  = var[11];

      n++;
    }
  }

  AbsPtr =  OptPar->QAbs; //initial set to no radiation damage


  Int_t N = OptPar->GetNPar();

  Double_t bins[N+1];
  bins[0] = 0.5*(3*OptPar->EPhoton[0]/eV - OptPar->EPhoton[1]/eV); //lower edge
  bins[N] = 0.5*(3*OptPar->EPhoton[N-1]/eV - OptPar->EPhoton[N-2]/eV); //upper edge
  
  for(int b = 1; b < N; b++){
    bins[b] = 0.5*(OptPar->EPhoton[b]/eV + OptPar->EPhoton[b-1]/eV);
  }
  
  QEffhst  = new TH1D("QEfficiency","",N,bins);
  LGRefl30  = new TH1D("LGRefl30","",N,bins);
  LGRefl45  = new TH1D("LGRefl45","",N,bins);
  LGRefl60  = new TH1D("LGRefl60","",N,bins);
  LGRefl90  = new TH1D("LGRefl90","",N,bins);
  CathIndR  = new TH1D("CathIndR","",N,bins);
  CathIndI  = new TH1D("CathIndI","",N,bins);
 
  for(int k = 0; k < N; k++){    
    QEffhst->SetBinContent(QEffhst->FindBin(OptPar->EPhoton[k]/eV),OptPar->QEff[k]);
    LGRefl30->SetBinContent(LGRefl30->FindBin(OptPar->EPhoton[k]/eV),OptPar->LGRefl30[k]);
    LGRefl45->SetBinContent(LGRefl45->FindBin(OptPar->EPhoton[k]/eV),OptPar->LGRefl45[k]);
    LGRefl60->SetBinContent(LGRefl60->FindBin(OptPar->EPhoton[k]/eV),OptPar->LGRefl60[k]);
    LGRefl90->SetBinContent(LGRefl90->FindBin(OptPar->EPhoton[k]/eV),OptPar->LGRefl90[k]);
    CathIndR->SetBinContent(CathIndR->FindBin(OptPar->EPhoton[k]/eV),OptPar->Cath_RIndR[k]);
    CathIndI->SetBinContent(CathIndI->FindBin(OptPar->EPhoton[k]/eV),OptPar->Cath_IIndR[k]);
  }  										 
										 
  TrackingReadout->StoreGuideOpticalPlots(LGRefl30,LGRefl45,LGRefl60,LGRefl90);
  TrackingReadout->StoreCathodeOpticalPlots(CathIndR, CathIndI);

  TrackingReadout->SetOpticalParameters(OptPar);
}


void MOLLEROptMaterial::DefineMaterials()
{

  // Define required materials

  G4double A;        // atomic mass
  G4double Z;        // atomic number
  G4double density;  // density

  G4double temperature; 
  G4double pressure;
  G4double fractionmass;

  G4String name;
  G4String symbol;
  
  G4int natoms; 
  G4int ncomponents;
  G4int nelements;
 
  //
  // Define general elements
  //

  // G4Element describes the properties of the atoms:
  // atomic number, number of nucleons, atomic mass, shell energy ...


  //---------------------------------------------------------
  name   = "Element_Hydrogen";
  symbol = "H"; 
  Z      = 1.;
  A      = 1.00794*g/mole;
  G4Element* elH = new G4Element(name, symbol, Z, A);
  //---------------------------------------------------------
  name   = "Element_Helium_3";
  symbol = "3He"; 
  Z      = 2.;
  A      = 3.016023*g/mole;
  //   G4Element* elHe3 = new G4Element(name, symbol, Z, A);
  
  G4Isotope* He3Iso = new G4Isotope("3HeIso", (G4int)Z,3,A);
  G4Element* elHe3 = new G4Element(name,symbol,1);
  elHe3->AddIsotope(He3Iso,100.0*perCent);
  
  //---------------------------------------------------------
  name   = "Element_Helium";
  symbol = "He"; 
  Z      = 2.;
  A      = 4.0026*g/mole;
  G4Element* elHe = new G4Element(name, symbol, Z, A);
  //---------------------------------------------------------
  
  name =  "Element_Lithium_6";
  symbol = "Li";
  Z = 3.;
  A = 6.0151*g/mole;
  G4Isotope* Li6Iso = new G4Isotope("6LiIso", (G4int)Z,6,A);
  G4Element* elLi6 = new G4Element(name, symbol,1);
  elLi6->AddIsotope(Li6Iso,100.0*perCent);
  
  
  //   name   = "Element_Boro";
  //   symbol = "B"; 
  //   Z      = 5.;
  //   A      = 10.811*g/mole;
  //   G4Element* elB = new G4Element(name, symbol, Z, A);
  //---------------------------------------------------------
  name   = "Element_Carbon";
  symbol = "C"; 
  Z      = 6.;
  A      = 12.011*g/mole;
  G4Element* elC = new G4Element(name, symbol, Z, A);
  //---------------------------------------------------------
  name   = "Element_Nitrogen";
  symbol = "N"; 
  Z      = 7.;
  A      = 14.01*g/mole;
  G4Element* elN = new G4Element(name, symbol, Z, A);
  //---------------------------------------------------------
  name   = "Element_Oxygen";
  symbol = "O"; 
  Z      = 8.;
  A      = 16.00*g/mole;
  G4Element* elO = new G4Element(name, symbol, Z, A);
  //---------------------------------------------------------
  //   name   = "Element_Fluorine";
  //   symbol = "F"; 
  //   Z      = 9.;
  //   A      = 19.00*g/mole;
  //   G4Element* elF = new G4Element(name, symbol, Z, A);
  //---------------------------------------------------------
  name   = "Element_Sodium";
  symbol = "Na"; 
  Z      = 11.;
  A      = 22.99*g/mole;
  G4Element* elNa = new G4Element(name, symbol, Z, A);
  //---------------------------------------------------------
  //   name   = "Element_Magnesium";
  //   symbol = "Mg"; 
  //   Z      = 12.;
  //   A      = 24.305*g/mole;
  //   G4Element* elMg = new G4Element(name, symbol, Z, A);
  //---------------------------------------------------------
  name   = "Element_Aluminum";
  symbol = "Al"; 
  Z      = 13.;
  A      = 26.981539*g/mole;
  G4Element* elAl = new G4Element(name, symbol, Z, A);
  //---------------------------------------------------------
  name   = "Element_Silicon";
  symbol = "Si"; 
  Z      = 14.;
  A      = 28.09*g/mole;
  G4Element* elSi  = new G4Element(name, symbol, Z, A);
  //---------------------------------------------------------
  //   name   = "Element_Chlorine";
  //   symbol = "Cl"; 
  //   Z      = 17.;
  //   A      = 35.453*g/mole;
  //   G4Element* elCl = new G4Element(name, symbol, Z, A);
  //---------------------------------------------------------
  name   = "Element_Argon";
  symbol = "Ar"; 
  Z      = 18.;
  A      = 39.95*g/mole;
  G4Element* elAr  = new G4Element(name, symbol, Z, A);
  //---------------------------------------------------------
  name   = "Element_Potassium";
  symbol = "K"; 
  Z      = 19.;
  A      = 39.0983*g/mole;
  G4Element* elK = new G4Element(name, symbol, Z, A);
  //---------------------------------------------------------
  name   = "Element_Calcium";
  symbol = "Ca"; 
  Z      = 20.;
  A      = 40.08*g/mole;
  G4Element* elCa = new G4Element(name, symbol, Z, A);
  //---------------------------------------------------------
  name   = "Element_Iron";
  symbol = "Fe";
  Z      = 26.;
  A      = 55.85*g/mole;
  G4Element* elFe  = new G4Element(name, symbol, Z, A);
  //---------------------------------------------------------
  name   = "Element_Xenon";
  symbol = "Xe";
  Z      = 52.;
  A      = 131.29*g/mole;
  G4Element* elXe  = new G4Element(name, symbol, Z, A);
  //---------------------------------------------------------
  
  //
  // define simple materials
  //
  //
  // The G4Material class describes the macroscopic properties of the matter:
  // density, state, temperature, pressure, radiation length, mean free path,
  // dE/dx ...
  // 
  // My name convention: all materials start with matXyz
  //
  // Sorted by Z
  
  // Liquid H2
  name     = "H2Liquid";
  density  = 0.0708*g/cm3;
  nelements = 1;
  G4Material* matLiquidHydrogen = new G4Material(name, density, nelements);
  matLiquidHydrogen -> AddElement(elH,1); 
  
  // Helium 3 Gas
  name     = "He3Gas";
  density  = 0.1340*mg/cm3;
  nelements = 1;
  G4Material* matHelium3Gas = new G4Material(name,density,nelements,kStateGas,273.15*kelvin,1.*atmosphere);
  matHelium3Gas->AddElement(elHe3,100.0*perCent);
  
  // Helium gas
  name     = "HeGas";
  density  = 0.1787*mg/cm3;
  nelements = 1;
  G4Material* matGasHelium = new G4Material(name,density,nelements, kStateGas, 273.15*kelvin,1.*atmosphere);
  matGasHelium ->AddElement(elHe,1);
  
  //Liquid  Helium 4
  name     = "HeLiquid";
  density  = 0.1249*g/cm3;
  nelements = 1;
  G4Material* matLiquidHelium = new G4Material(name, density, nelements);
  matLiquidHelium -> AddElement(elHe,1); 
  
  
  // Al material
  name     = "Aluminum";
  density  = 2.700*g/cm3;
  nelements = 1;
  G4Material* matAl = new G4Material(name,density,nelements);
  matAl -> AddElement(elAl,1);
  
  
  // gaseous Argon
  name      = "ArgonGas";
  density   = 1.7836*mg/cm3 ;       // STP
  nelements = 1;
  G4Material* matArgonGas = new G4Material(name,density,nelements, kStateGas, 273.15*kelvin,1.*atmosphere);
  matArgonGas ->AddElement(elAr, 1);
  
  // Lithium 6 material
  name =  "Lithium6";
  density = 0.534*g/cm3;
  // Z = 3.;
  // A = 6.0151*g/mole;
  nelements = 1;
  // G4Material* matLithium6 = new G4Material(name, Z, A, density);
  G4Material* matLithium6 = new G4Material(name,density,nelements);
  matLithium6 ->AddElement(elLi6, 1);


  // Iron material
  //   name    = "Iron";
  //   A       = 55.85*g/mole;
  //   Z       = 26.;
  //   density = 7.87*g/cm3;
  //   G4Material* matIron = new G4Material(name,Z,A,density);
  
  // Copper material
  name    = "Copper";
  A       = 63.54*g/mole;
  Z       = 29.;
  density = 8.96*g/cm3;
  G4Material* matCopper = new G4Material(name,Z,A,density);

  // Tin material
  name    = "Tin";
  A       = 118.69*g/mole;
  Z       = 50.;
  density =  7.28*g/cm3;
  G4Material* matSn = new G4Material(name,Z,A,density);

  // Lead material
  name    = "Lead";
  A       = 207.19*g/mole;
  Z       = 82.;
  density = 11.35*g/cm3;
  G4Material* matPb = new G4Material(name,Z,A,density);




  // photocathode material, approximated as elemental cesium
  name      = "Photocathode";
  density   = 5.0*g/cm3; // true??
  nelements = 1;
  G4Material * matPhotocathode =  new G4Material(name,density,nelements);
  matPhotocathode -> AddElement(elK, 1);

  //=============================================================
  //
  // define a material from elements.   case 1: chemical molecule
  //
  //=============================================================




  // Xe gas
  name        = "XenonGas";
  density     = 5.458*mg/cm3;
  ncomponents = 1;
  G4Material* matXe =  new G4Material(name,density, ncomponents, kStateGas,273.15*kelvin,1.*atmosphere);
  matXe -> AddElement(elXe,1);
             
  // CO2 , STP
  name        = "CO2";
  density     = 1.818*mg/cm3;
  ncomponents = 2;
  G4Material* matCO2 = new G4Material(name,density, ncomponents, kStateGas,273.15*kelvin,1.*atmosphere);
  matCO2->AddElement(elC, natoms=1);
  matCO2->AddElement(elO, natoms=2);
	      
  name        = "N2";
  density     = 1.166*mg/cm3;
  ncomponents = 1;
  G4Material* matN2 = new G4Material(name,density, ncomponents, kStateGas,293.0*kelvin,1.*atmosphere);
  matN2->AddElement(elN, natoms=2);

  
  // Water
  name        = "Water" ;
  density     = 1.000*g/cm3;
  ncomponents = 2;
  G4Material* matH2O = new G4Material(name,density ,ncomponents);
  matH2O -> AddElement(elH,natoms=2);
  matH2O -> AddElement(elO,natoms=1);
  matH2O -> GetIonisation() -> SetMeanExcitationEnergy(75.0*eV);
  
  // Scintillator
  name        = "Scintillator";
  density     = 1.032*g/cm3;
  ncomponents = 2;
  G4Material* matScint = new G4Material(name, density, ncomponents);
  matScint->AddElement(elC, natoms=9);
  matScint->AddElement(elH, natoms=10);


  // Quartz SiO2 (e.g. Spectrosil 2000), optical properties will be added 
  name        = "Quartz";
  density     = 2.203*g/cm3;
  ncomponents = 2;
  G4Material* matQuartz = new G4Material(name,density, ncomponents);
  matQuartz->AddElement(elSi, natoms=1);
  matQuartz->AddElement(elO , natoms=2);

  // Quartz SiO2 (e.g. Spectrosil 2000) without optical properties
  name        = "SiO2";
  density     = 2.200*g/cm3;
  ncomponents = 2;
  G4Material* matSiO2 = new G4Material(name,density, ncomponents);
  matSiO2->AddElement(elSi, natoms=1);
  matSiO2->AddElement(elO , natoms=2);


  // SiElast_Glue  The glue used to glue together the quartz pieces
  name        = "SiElast_Glue";
  density     = 2.200*g/cm3;
  ncomponents = 2;
  G4Material* matSiElast = new G4Material(name,density, ncomponents);
  matSiElast->AddElement(elSi, natoms=1);
  matSiElast->AddElement(elO , natoms=2);

  // Lime Glass
  name        = "LimeGlass";
  density     = 2.200*g/cm3;
  ncomponents = 2;
  G4Material* matLimeGlass = new G4Material(name,density, ncomponents);
  matLimeGlass->AddElement(elSi, natoms=1);
  matLimeGlass->AddElement(elO , natoms=2);


  //Mylar
  name        = "Mylar";
  density     = 1.397 *g/cm3;
  ncomponents = 3;
  G4Material* matMylar = new G4Material(name,density , ncomponents);
  matMylar -> AddElement(elH, natoms= 8);
  matMylar -> AddElement(elC, natoms=10);
  matMylar -> AddElement(elO, natoms= 4);

  //Mirror
  name        = "Mirror";
  density     = 1.397 *g/cm3;
  ncomponents = 3;
  G4Material* matMirror = new G4Material(name,density , ncomponents);
  matMirror -> AddElement(elH, natoms= 8);
  matMirror -> AddElement(elC, natoms=10);
  matMirror -> AddElement(elO, natoms= 4);


  // Tyvek (High density Polyethylene)
  //  (...-CH2-CH2-...)*n
  name        = "Tyvek";
  density     = 0.96 *g/cm3;
  ncomponents = 2;
  G4Material* matTyvek = new G4Material(name,density , ncomponents);
  matTyvek -> AddElement(elH, natoms= 2);
  matTyvek -> AddElement(elC, natoms= 1);
  
  // Kevlar
  // (-NH-C6H4-NH-CO-C6H4-CO-)*n
  name        =	"Kevlar";
  density     = 1.44 *g/cm3;
  ncomponents = 4;
  G4Material* matKevlar = new G4Material(name,density , ncomponents);
  matKevlar -> AddElement(elH, natoms=10 );
  matKevlar -> AddElement(elC, natoms=14);
  matKevlar -> AddElement(elO, natoms= 2);
  matKevlar -> AddElement(elN, natoms= 2);

  //
  //    ---  H        O  -----
  //        -N-(CH2)5-C-
  //  
  name        = "Nylon";
  density     = 0.805*g/cm3;
  ncomponents = 4;
  G4Material* matNylon = new G4Material(name,density , ncomponents);
  matNylon -> AddElement(elH, natoms=11 );
  matNylon -> AddElement(elC, natoms= 6);
  matNylon -> AddElement(elO, natoms= 1);
  matNylon -> AddElement(elN, natoms= 1);



  //     H H 
  //  ---C-C--- 
  //     H COOCH3
  name        = "Acrylic";
  density     = 1.14*g/cm3;
  ncomponents = 3;
  G4Material* matAcrylic = new G4Material(name,density , ncomponents);
  matAcrylic -> AddElement(elH, natoms= 6);
  matAcrylic -> AddElement(elC, natoms= 4);
  matAcrylic -> AddElement(elO, natoms= 2);

  //   
  //  Nema grade G10 or FR4
  //  
  name        = "NemaG10";
  density     = 1.70*g/cm3;
  ncomponents = 4;
  G4Material* matG10 = new G4Material(name,density , ncomponents);
  matG10 -> AddElement(elSi, natoms=1);
  matG10 -> AddElement(elO , natoms=2);
  matG10 -> AddElement(elC , natoms=3);
  matG10 -> AddElement(elH , natoms=3);


  //======================================================================
  //
  // define a material from elements.   case 2: mixture by fractional mass
  //
  //======================================================================

  //Neutron Beam Monitor gas mixture

  name        = "NMonitorGas";
  density     = 0.373*mg/cm3;
  ncomponents = 3;
  G4Material* matNMonGas = new G4Material(name,density,ncomponents);
  matNMonGas -> AddMaterial(matHelium3Gas,fractionmass=0.01);
  matNMonGas -> AddMaterial(matGasHelium,fractionmass=0.21);
  //   matNMonGas -> AddMaterial(matCO2,fractionmass=0.78);
  matNMonGas -> AddMaterial(matN2,fractionmass=0.78);

  // Air material: Air 18 degr.C and 58% humidity
  name        = "Air";
  density     = 1.214*mg/cm3;
  ncomponents = 4;
  G4Material* matAir = new G4Material(name,density,ncomponents);
  matAir -> AddElement(elN,  fractionmass=0.7494);
  matAir -> AddElement(elO,  fractionmass=0.2369);
  matAir -> AddElement(elAr, fractionmass=0.0129);
  matAir -> AddElement(elH,  fractionmass=0.0008);

  double dp = 0.024;
  double pinc = 0.0008;

  G4Material* matN3HeTDGas;
  for(int i = 0; i < 61; i++){
    
    pressure = 0.476 - dp + pinc*i;      
    name        = Form("N3HeTDGas%d",(G4int)(pressure*10000));//10-i);
    density     = 101325*pressure/808.16/1000*mg/cm3;          
    ncomponents = 1;
    // matN3HeTDGas = new G4Material(name,density,ncomponents,kStateGas,293.15*kelvin,pressure*atmosphere);
    matN3HeTDGas = new G4Material(name,density,ncomponents,kStateGas,0.0*kelvin,pressure*atmosphere);
    matN3HeTDGas->AddElement(elHe3,100.0*perCent);
    // userInfo->SetPressure(pressure);    
  }
  pressure = 0.5;      
  name        = Form("N3HeTDGas%d",(G4int)(pressure*1000));//10-i);
  density     = 101325*pressure/808.16/1000*mg/cm3;          
  ncomponents = 1;
  // matN3HeTDGas = new G4Material(name,density,ncomponents,kStateGas,293.15*kelvin,pressure*atmosphere);
  matN3HeTDGas = new G4Material(name,density,ncomponents,kStateGas,0.0*kelvin,pressure*atmosphere);
  matN3HeTDGas->AddElement(elHe3,100.0*perCent);


  // Kapton
  name        = "Kapton";
  density     = 1.42*g/cm3;
  ncomponents = 4;
  G4Material* matKapton = new G4Material(name,density, ncomponents);
  matKapton->AddElement(elH, fractionmass = 0.0273);
  matKapton->AddElement(elC, fractionmass = 0.7213);
  matKapton->AddElement(elN, fractionmass = 0.0765);
  matKapton->AddElement(elO, fractionmass = 0.1749);
  
  
   
  // Polyethylene
  name        = "Polyethylene";
  density     = 0.94 * g/cm3;
  ncomponents = 2;
  G4Material* matPolyethylene = new G4Material(name,density, ncomponents);
  matPolyethylene -> AddElement(elH, fractionmass=0.14);
  matPolyethylene -> AddElement(elC, fractionmass=0.86);
  
  // Polyacrylate
  name        = "Polyacrylate";
  density     = 1.19 * g/cm3;
  ncomponents = 3;
  G4Material* matPolyacrylate = new G4Material(name,density,ncomponents);
  matPolyacrylate -> AddElement(elH, fractionmass=0.08);
  matPolyacrylate -> AddElement(elC, fractionmass=0.60);
  matPolyacrylate -> AddElement(elO, fractionmass=0.32);
 
  // VDC ArCO2 80/20
  name        = "ArCO2";
  density     = 0.0018*g/cm3; // to be checked
  ncomponents = 2;
  G4Material* matArCO2 = new G4Material(name,density,ncomponents);
  matArCO2->AddMaterial(matArgonGas, fractionmass = 0.8);
  matArCO2->AddMaterial(matCO2,      fractionmass = 0.2);


  // ShieldingConcrete: must  check recipe for concrete

  name        = "ShieldingConcrete";
  density     = 2.5*g/cm3;
  ncomponents = 6;
  G4Material* matConcrete = new G4Material(name,density,ncomponents);
  matConcrete->AddElement(elO,  fractionmass = 0.520);
  matConcrete->AddElement(elSi, fractionmass = 0.325);
  matConcrete->AddElement(elCa, fractionmass = 0.060);
  matConcrete->AddElement(elNa, fractionmass = 0.015);
  matConcrete->AddElement(elFe, fractionmass = 0.040);
  matConcrete->AddElement(elAl, fractionmass = 0.040);


  // material for the collimators: High Leaded Tin Bronze 
  // Copper Alloy No. C94300
  // see http://www/anchorbronze.com/c94300.html
  name        = "CDA943";
  density     = 9.29 * g/cm3;
  ncomponents = 3;
  G4Material* matCollimator = new G4Material(name,density,ncomponents);
  matCollimator-> AddMaterial(matCopper, fractionmass = 0.695);
  matCollimator-> AddMaterial(matPb    , fractionmass = 0.25);
  matCollimator-> AddMaterial(matSn    , fractionmass = 0.055);
  


   // TRT_CH2
   name        = "CH2";
   density     = 0.935*g/cm3;
   ncomponents = 2;
   G4Material* matCH2 = new G4Material(name, density, ncomponents);
   matCH2->AddElement(elC, natoms=1);
   matCH2->AddElement(elH, natoms=2);


   //vacuum
   name        = "Vacuum";
   A           = 1.01*g/mole;
   Z           = 1.;
   density     = 1.e-25 *g/cm3;
   pressure    = 3.e-18*pascal;
   temperature = 2.73*kelvin;
   G4Material* matVacuum = new G4Material("Vacuum", Z, A, density,kStateGas,temperature,pressure);
   pressure = matVacuum->GetPressure();
   //               matVacuum -> AddElement(elHe,  fractionmass=1);
   
   
   //========================================
   // Hydrocarbones, metane and others
   //========================================
   
   // CH4: Metane, STP
   name        = "Methane";
   density     = 0.7174*mg/cm3 ;
   ncomponents = 2;
   G4Material* matMetane = new G4Material(name,density,ncomponents);
   matMetane->AddElement(elC, natoms= 1) ;
   matMetane->AddElement(elH, natoms= 4) ;
   
   // C3H8: Propane, STP
   name        = "Propane";
   density     = 2.005*mg/cm3 ;
   ncomponents = 2;
   G4Material* matPropane = new G4Material(name,density,ncomponents);
   matPropane->AddElement(elC, natoms= 3) ;
   matPropane->AddElement(elH, natoms= 8) ;
   
   // C4H10 : iso-Butane (methylpropane), STP
   name        = "IsoButane";
   density     = 2.67*mg/cm3 ;
   ncomponents = 2;
   G4Material* matIsobutane = new G4Material(name,density,ncomponents);
   matIsobutane->AddElement(elC,natoms=  4) ;
   matIsobutane->AddElement(elH,natoms= 10) ;
   
   // C2H6 : Ethane, STP
   name        = "Ethane";
   density     = 1.356*mg/cm3 ;
   ncomponents = 2;
   G4Material* matEthane = new G4Material(name,density,ncomponents);
   matEthane->AddElement(elC, natoms= 2) ;
   matEthane->AddElement(elH, natoms= 6) ;
   


   //============================================================================================
   //                          Optical Propeties
   //============================================================================================



   //===========================
   //  Optical Properties of Air
   //===========================

   G4int nEntries = OptPar->GetNPar();

   G4double  RefractiveIndex_Air[nEntries];
   for(int n = 0; n < nEntries; n++)
     RefractiveIndex_Air[n] = 1.0003; //probably needs more exact values

   G4double  AbsorptionCoeff_Air[nEntries];
   for(int n = 0; n < nEntries; n++)
     AbsorptionCoeff_Air[n] = 10e1*m; //same here

   G4MaterialPropertiesTable* myMPT_Air = new G4MaterialPropertiesTable();
   myMPT_Air->AddProperty("RINDEX",    OptPar->EPhoton, RefractiveIndex_Air, nEntries);
   myMPT_Air->AddProperty("ABSLENGTH", OptPar->EPhoton, AbsorptionCoeff_Air, nEntries);   
   matAir->SetMaterialPropertiesTable(myMPT_Air);

   //====================================
   //  Optical Properties of Fused Silica
   //====================================
   
   // Fused Silica (Spectrosil 2000) for the Cerenkov Detector

   RefractiveIndex_Quartz = new G4double[nEntries];
    // G4double RefractiveIndex_Quartz[nEntries];

   //Sellmeier Coefficients:
   //Ref.: K Imai, Ph.D. thesis

   //I.H.Malitson 1965 values
   double b1 = 0.6961663;
   double b2 = 0.4079426;
   double b3 = 0.8974794;
   
   double c1 = 0.004679148;
   double c2 = 0.01351206;
   double c3 = 97.93400;

   //Heraeus data sheet values
   // double b1 = 0.473115591;  
   // double b2 = 0.631038719;  
   // double b3 = 0.906404498;  
   
   // double c1 = 0.0129957170; 
   // double c2 = 0.00412809220;
   // double c3 = 98.7685322;   

   double x;
   // double a = 0.0072973525693;
   // double b =b 1; // set this equal to 1 for a GeV electrons
   
   for(int i = 0; i < nEntries; i++){
     x = 1.242/(OptPar->EPhoton[i]/eV);
     // RefractiveIndex_Quartz[i] = TMath::Sqrt(1 + b1*x*x/(x*x - c1) + b2*x*x/(x*x - c2) + b3*x*x/(x*x - c3));
     RefractiveIndex_Quartz[i] = 1.438 + (.01197*OptPar->EPhoton[i]/eV) - (.001955*OptPar->EPhoton[i]*OptPar->EPhoton[i]/eV/eV) + (.0004793*OptPar->EPhoton[i]*OptPar->EPhoton[i]*OptPar->EPhoton[i]/eV/eV/eV);

   }   
   myMPT_FusedSilica = new G4MaterialPropertiesTable();
   myMPT_FusedSilica->AddProperty("RINDEX",    OptPar->EPhoton, RefractiveIndex_Quartz , nEntries);
   myMPT_FusedSilica->AddProperty("ABSLENGTH", OptPar->EPhoton, AbsPtr, nEntries);
   //myMPT_FusedSilica->AddProperty("REFLECTIVITY", OptPar->EPhoton,OptPar->QRefl, nEntries);
   matQuartz->SetMaterialPropertiesTable(myMPT_FusedSilica);


   // G4double RefractiveIndex_Si[nEntries];
   // for(int i = 0; i < nEntries; i++){
   //   RefractiveIndex_Si[i] = 1.438 + (.01197*OptPar->EPhoton[i]/eV) - (.001955*OptPar->EPhoton[i]*OptPar->EPhoton[i]/eV/eV) + (.0004793*OptPar->EPhoton[i]*OptPar->EPhoton[i]*OptPar->EPhoton[i]/eV/eV/eV);
   // }
   // G4MaterialPropertiesTable* myMPT_Si = new G4MaterialPropertiesTable();
   // myMPT_Si->AddProperty("RINDEX",    OptPar->EPhoton, RefractiveIndex_Si , nEntries);
   // myMPT_Si->AddProperty("ABSLENGTH", OptPar->EPhoton, AbsPtr, nEntries);
   // //myMPT_Si->AddProperty("REFLECTIVITY", OptPar->EPhoton,OptPar->QRefl, nEntries);
   // matSiO2->SetMaterialPropertiesTable(myMPT_Si);
   
   

   if(GetPixellatedRelativeRate())
     CalculateAbsorptionLengths(elSi,elO);

}

void MOLLEROptMaterial::SetRadDamageLevel(G4int dam)
{
  if(dam == 70){
    AbsPtr = OptPar->QAbs70;  //70 MRad damage
    myMPT_FusedSilica->RemoveProperty("ABSLENGTH");
    myMPT_FusedSilica->AddProperty("ABSLENGTH", OptPar->EPhoton, AbsPtr, OptPar->GetNPar());    
  }
  if(dam == 170){
    AbsPtr = OptPar->QAbs170; //170 MRad damage
    myMPT_FusedSilica->RemoveProperty("ABSLENGTH");
    myMPT_FusedSilica->AddProperty("ABSLENGTH", OptPar->EPhoton, AbsPtr, OptPar->GetNPar());    
  }
  //otherwise no rad damage
}

G4Material* MOLLEROptMaterial::GetMaterial(G4String material)
{
  G4Material* pttoMaterial = G4Material::GetMaterial(material); 

  
  return pttoMaterial; 
}
