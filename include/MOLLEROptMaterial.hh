#ifndef MOLLEROptMaterial_H
#define MOLLEROptMaterial_H 1

#include "cpp_include.h"
#include "Geant4_include.hh" 
#include "Root_include.h"
#include "MOLLEROptTrackingReadout.hh"

class OptParam{
  
public:

  OptParam(int n) {
    npar = n;
    EPhoton = new Double_t[npar];
    QEff = new Double_t[npar];
    QRefl = new Double_t[npar];
    Cath_RIndR = new Double_t[npar];
    Cath_IIndR = new Double_t[npar];
    QTrans = new Double_t[npar];
    LGRefl30  = new Double_t[npar];
    LGRefl45 = new Double_t[npar];
    LGRefl60  = new Double_t[npar];
    LGRefl90  = new Double_t[npar];
    QAbs = new Double_t[npar];
    QAbs70 = new Double_t[npar];
    QAbs170 = new Double_t[npar];
  };
  
  ~OptParam(){
    delete EPhoton;
    delete QEff;
    delete QRefl;
    delete Cath_RIndR;
    delete Cath_IIndR;
    delete QTrans;
    delete LGRefl30;
    delete LGRefl45;
    delete LGRefl60;
    delete LGRefl90;
    delete QAbs;
    delete QAbs70;
    delete QAbs170;
  };
  
  Double_t *EPhoton;
  Double_t *QEff;
  Double_t *Cath_RIndR;
  Double_t *Cath_IIndR;
  Double_t *QRefl;
  Double_t *QTrans;
  Double_t *LGRefl30;
  Double_t *LGRefl45;
  Double_t *LGRefl60;
  Double_t *LGRefl90;
  Double_t *QAbs;
  Double_t *QAbs70;
  Double_t *QAbs170;
  //vector<Double_t*> QAbsV;
  vector<Double_t> QAbs_X;
  vector<Double_t> QAbs_Y;
  vector<Double_t> QAbs_dX;
  vector<Double_t> QAbs_dY;
  vector<G4Material*> CellMaterial;
  

  Int_t npar;
  Int_t GetNPar() {return npar;};
  void AddNewQAbs(Double_t x, Double_t y, Double_t dx, Double_t dy, G4Material* mat)
  {
    if(!mat) return;
    
    //QAbsV.push_back(abs);
    QAbs_X.push_back(x);
    QAbs_Y.push_back(y);
    QAbs_dX.push_back(dx);
    QAbs_dY.push_back(dy);
    CellMaterial.push_back(mat);
    
  };

};

class MOLLEROptMaterial
{ 
public:
  MOLLEROptMaterial(MOLLEROptTrackingReadout*);
  ~ MOLLEROptMaterial();

public:
  void  DefineMaterials();
  G4Material* GetMaterial(G4String);

  void SetOpticalParameters();
  OptParam* GetOpticalParametersTable(){return OptPar;};
  void SetRadDamageLevel(G4int dam);
  void SetSegRadDamage();

private:

  G4bool GetPixellatedRelativeRate();
  void CalculateAbsorptionLengths(G4Element*, G4Element*);
  
  OptParam *OptPar;
  Double_t *AbsPtr;
  Int_t NOptPar;
  G4MaterialPropertiesTable* myMPT_FusedSilica;
  vector <G4MaterialPropertiesTable*> QuartzCells_MPT;
  G4bool SegRadDamgeFlag;

  MOLLEROptTrackingReadout* TrackingReadout;

  G4double *RefractiveIndex_Quartz;

  TH2D *PixellatedRelativeRate;
  TH1D* QEffhst;  
  TH1D* LGRefl30;  
  TH1D* LGRefl45;  
  TH1D* LGRefl60;  
  TH1D* LGRefl90;  
  TH1D* CathIndR;  
  TH1D* CathIndI;  


};

#endif
