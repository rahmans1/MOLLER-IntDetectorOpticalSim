Double_t langaufun(Double_t *x, Double_t *par);  
TF1 *langaufit(TH1D *his, Double_t *fitrange, Double_t *startvalues, Double_t *parlimitslo, Double_t *parlimitshi, Double_t *fitparams, Double_t *fiterrors, Double_t *ChiSqr, Int_t *NDF);
Int_t langaupro(Double_t *params, Double_t &maxx, Double_t &FWHM);
void DoFit(TH1D *hst, Double_t *fitP, Double_t *fitE);
Int_t FindGraph(Int_t fA, Int_t hR);

std::vector <TGraphErrors*> fA_PEmean;
std::vector <TGraphErrors*> fA_PEmean_hR2;
std::vector <TGraphErrors*> fA_PEmean_hR3;
std::vector <TGraphErrors*> fA_Exnse;
std::vector <TH2D*> fAbA_PEmean;
std::vector <TCanvas*> FitHist;


void ExtractResults(TString *dir = 0)
{
  gSystem->Load("libMOLLEROptDictionaries.so");

  TString datadir;
  if(dir) {
    datadir = dir->Data();
    TString rootfiles;
    rootfiles.Form("ls %s/MOLLEROpt*.root > files.dat",datadir.Data());
    system(rootfiles.Data());
  }
  

  std::ifstream rfiles("files.dat");

  if(!rfiles.is_open()) { cout << "Cannot find root file list files.dat. Either specify a directory or the files.dat file." << endl; return;} 
  
  std::string line;
  TFile *file;
  TVectorD *ba;
  TVectorD *fa;
  TVectorD *hr;
  
  TH1D *hst, *tmp;
  TSpectrum *s = new TSpectrum(1);
  Int_t m;
  TString runID, tmpStr, tok;
  Ssiz_t from = 0;
  Double_t fitP[4], fitE[4];
  Int_t fAmin = 90;
  Int_t fAmax = 0;
  Int_t bAmin = 90;
  Int_t bAmax = 0;
  Int_t oFmin = 10;
  Int_t oFmax = 0;
  Int_t fA, bA, oF;
  
  while(std::getline(rfiles, line)){
    from = 0;
    runID = line.data();
    
    while(runID.Tokenize(tok,from,"_")){
      
      if(tok.Contains("fA")){
    	tok.Remove(0,2);
    	fA = tok.Atoi();	
	// cout << "fA = " << fA << endl;
	if(fA < fAmin) fAmin = fA;
	if(fA > fAmax) fAmax = fA;
      }
      if(tok.Contains("bA")){
    	tok.Remove(0,2);
    	bA = tok.Atoi();	
	// cout << "bA = " << bA << endl;
	if(bA < bAmin) bAmin = bA;
	if(bA > bAmax) bAmax = bA;
      }
      if(tok.Contains("oF")){
    	tok.Remove(0,2);
    	oF = tok.Atoi();	
	// cout << "oF = " << oF << endl;
	if(oF < oFmin) oFmin = oF;
	if(oF > oFmax) oFmax = oF;
      }
    }
  }
  rfiles.clear();
  rfiles.seekg(0,ios::beg);

  Int_t fAbins = fAmax - fAmin +1;
  Int_t bAbins = bAmax - bAmin +1;
  Int_t oFbins = (oFmax - oFmin)/2 +1;

  cout << "fAmin = " << fAmin  << " fAmax = " << fAmax  << " fAbins = " << fAbins << endl;
  cout << "bAmin = " << bAmin  << " bAmax = " << bAmax  << " bAbins = " << bAbins << endl;
  cout << "oFmin = " << oFmin  << " oFmax = " << oFmax  << " oFbins = " << oFbins << endl << endl;

  for(int r = 0; r < 3; r++){
    fAbA_PEmean.push_back(new TH2D(Form("PEMean_hR%d",r+1),"",fAbins,fAmin-0.5,fAmax+0.5,bAbins,bAmin-0.5,bAmax+0.5));
  }

 
  
  while(std::getline(rfiles, line)){

    from = 0;
    file = TFile::Open(line.data());

    hr = (TVectorD*)file->Get("HitRegion");
      
    // cout << line.data() << endl;
    tmpStr = line.data();
    tmpStr = tmpStr.ReplaceAll("MOLLEROpt_","");
    runID = tmpStr.ReplaceAll(".root","");      
    ba = (TVectorD*)file->Get("LowerConeBackFaceAngle");
    fa = (TVectorD*)file->Get("LowerConeFrontFaceAngle");
    
    tmp = (TH1D*)file->Get("CathodeEventsDistrHist");
    
    hst = (TH1D*)tmp->Clone(Form("CEH_%s",runID.Data()));
    hst->SetTitle("Photoelectron Distribution");
    hst->GetXaxis()->SetTitle("Photoelectrons");
    hst->SetDirectory(0);

    m = FindGraph((*fa)[0],(*hr)[0]);


    if(m >= 0){

      if((*hr)[0] == 1){
	DoFit(hst,fitP,fitE);
	
	fA_PEmean[m]->SetPoint(fA_PEmean[m]->GetN(),(*ba)[0],fitP[1]);
	fA_PEmean[m]->SetPointError(fA_PEmean[m]->GetN()-1,0,fitE[1]);
	
	fA_Exnse[m]->SetPoint(fA_Exnse[m]->GetN(),(*ba)[0],pow(fitP[3]/fitP[1],2));
	fA_Exnse[m]->SetPointError(fA_Exnse[m]->GetN()-1,0,2*pow(fitP[3]/fitP[1],2)*sqrt(fitE[3]*fitE[3]/fitP[3]/fitP[3] + fitE[1]*fitE[1]/fitP[1]/fitP[1]));
	fAbA_PEmean[(*hr)[0]-1]->SetBinContent(fAbA_PEmean[(*hr)[0]-1]->FindBin((*fa)[0],(*ba)[0]),fitP[1]);
	// cout << "binx = " << (*fa)[0]  << "biny = " << (*ba)[0]  << "pe = " << fitP[1] << endl;
      }
      else if((*hr)[0] == 2){
	
	fA_PEmean_hR2[m]->SetPoint(fA_PEmean_hR2[m]->GetN(),(*ba)[0],hst->GetMean());
	fA_PEmean_hR2[m]->SetPointError(fA_PEmean_hR2[m]->GetN()-1,0,hst->GetMeanError());
	
	fAbA_PEmean[(*hr)[0]-1]->SetBinContent(fAbA_PEmean[(*hr)[0]-1]->FindBin((*fa)[0],(*ba)[0]),hst->GetMean());
      }    
      else if((*hr)[0] == 3){
	
	fA_PEmean_hR3[m]->SetPoint(fA_PEmean_hR3[m]->GetN(),(*ba)[0],hst->GetMean());
	fA_PEmean_hR3[m]->SetPointError(fA_PEmean_hR3[m]->GetN()-1,0,hst->GetMeanError());
	
	fAbA_PEmean[(*hr)[0]-1]->SetBinContent(fAbA_PEmean[(*hr)[0]-1]->FindBin((*fa)[0],(*ba)[0]),hst->GetMean());	
      }
    }
    
    
    file->Close("R");    
  }
  rfiles.close();

  TFile* oFile = new TFile("Results.root","RECREATE");
  oFile->cd();
  for(int f = 0; f < FitHist.size(); f++){
    FitHist[f]->Write();
  }

  for(int f = 0; f < fA_PEmean.size(); f++){
    fA_PEmean[f]->Write();
  }
  for(int f = 0; f < fA_PEmean_hR2.size(); f++){
    fA_PEmean_hR2[f]->Write();
  }
  for(int f = 0; f < fA_PEmean_hR3.size(); f++){
    fA_PEmean_hR3[f]->Write();
  }
  for(int f = 0; f < fA_Exnse.size(); f++){
    fA_Exnse[f]->Write();
  }
  for(int f = 0; f < fAbA_PEmean.size(); f++){
    fAbA_PEmean[f]->Write();
  }
  
  oFile->Close();
  for(int f = 0; f < FitHist.size(); f++){
    delete FitHist[f];
  }
}


Int_t FindGraph(Int_t fA, Int_t hR)
{
  TString name;

  if(hR == 1){
    for(int k = 0; k < fA_PEmean.size(); k++){
      if(fA_PEmean[k]){
	name = fA_PEmean[k]->GetName();
	if(name.Contains(Form("fA%d",fA)) && name.Contains(Form("hR%d",hR))){
	  return k;
	}
      }
    }
    TGraphErrors* gr = new TGraphErrors();
    TGraphErrors* gr2 = new TGraphErrors();
    gr->SetName(Form("PEMean_fA%d_hR%d",fA,hR));
    gr2->SetName(Form("Exnse_fA%d_hR%d",fA,hR));
    //gr->GetYaxis()->SetRangeUser(0,40);
    gr->SetMarkerStyle(21);
    gr2->SetMarkerStyle(21);
    gr->GetXaxis()->SetTitle("Lower Funnel Downstream Angle [Deg]");
    gr2->GetXaxis()->SetTitle("Lower Funnel Downstream Angle [Deg]");
    fA_PEmean.push_back(gr);
    fA_Exnse.push_back(gr2);
    return fA_PEmean.size()-1;
  }
  else if(hR == 2){

    for(int k = 0; k < fA_PEmean_hR2.size(); k++){
      if(fA_PEmean_hR2[k]){
	name = fA_PEmean_hR2[k]->GetName();
	if(name.Contains(Form("fA%d",fA)) && name.Contains(Form("hR%d",hR))){
	  return k;
	}
      }
    }
    TGraphErrors* gr = new TGraphErrors();
    gr->SetName(Form("PEMean_fA%d_hR%d",fA,hR));
    gr->SetMarkerStyle(21);
    gr->GetXaxis()->SetTitle("Lower Funnel Downstream Angle [Deg]");
    fA_PEmean_hR2.push_back(gr);
    return fA_PEmean_hR2.size()-1;

  }
  else if(hR == 3){

    for(int k = 0; k < fA_PEmean_hR3.size(); k++){
      if(fA_PEmean_hR3[k]){
	name = fA_PEmean_hR3[k]->GetName();
	if(name.Contains(Form("fA%d",fA)) && name.Contains(Form("hR%d",hR))){
	  return k;
	}
      }
    }
    TGraphErrors* gr = new TGraphErrors();
    gr->SetName(Form("PEMean_fA%d_hR%d",fA,hR));
    gr->SetMarkerStyle(21);
    gr->GetXaxis()->SetTitle("Lower Funnel Downstream Angle [Deg]");
    fA_PEmean_hR3.push_back(gr);
    return fA_PEmean_hR3.size()-1;

  }

  

  return -1;
  
}

void DoFit(TH1D *hst, Double_t *fitR, Double_t *fitE)
{

  //cout << "mean pe = " <<  s->GetPositionX()[0] << endl;
  TCanvas *cnv = new TCanvas();
  cnv->cd();
  hst->Draw();
 
  //Find peaks:
  int npeaks = 1;
  Double_t peaks_m[npeaks];
  Double_t peaks_h[npeaks];
  TSpectrum *s = new TSpectrum(2*npeaks);
  Int_t nfound2 = s->Search(hst,5,"",0.01);
  peaks_m[0] =  s->GetPositionX()[0];
  peaks_h[0] =  s->GetPositionY()[0];
  //cout << "Event peak located at " << peaks_m[0] << " height = " << peaks_h[0] << endl;    
  cnv->Update();

  printf("Fitting...\n");
 
  // Setting fit range and start values
  Double_t fr[2];
  Double_t sv[4], pllo[4], plhi[4], fp[4], fpe[4];
  fr[0] = peaks_m[0] - 10;
  fr[1] = peaks_m[0] + 10;

  sv[0] = fr[1] - fr[0];
  sv[1] = peaks_m[0];
  sv[2] = peaks_h[0]*sv[0];
  sv[3] = peaks_m[0] - fr[0];
 
  pllo[0]=sv[0]/100;
  plhi[0]=sv[0]*4;
   
  pllo[1]=sv[1]-2*sv[3];
  plhi[1]=sv[1]+2*sv[3];
  
  pllo[2]=sv[2]/10;
  plhi[2]=sv[2]*10;
  
  pllo[3]=sv[3]/4;
  plhi[3]=sv[3]*4;
 
  Double_t chisqr;
  Int_t    ndf;
  TF1 *fitsnr = langaufit(hst,fr,sv,pllo,plhi,fp,fpe,&chisqr,&ndf);
 
  Double_t SNRPeak, SNRFWHM;
  langaupro(fp,SNRPeak,SNRFWHM);
 
  printf("Fitting done\nPlotting results...\n");

  for(int n = 0; n < 4; n++){
    fitR[n] = fp[n];
    fitE[n] = fpe[n];
    // cout << fp[n] << endl;
    // cout << fpe[n] << endl;
    
  }
 
  // Global style settings
  gStyle->SetOptStat(1111);
  gStyle->SetOptFit(111);
  gStyle->SetLabelSize(0.03,"x");
  gStyle->SetLabelSize(0.03,"y");
  
  hst->GetXaxis()->SetRange(0,100);
  hst->Draw();
  fitsnr->Draw("lsame");
  FitHist.push_back(cnv);
  
}


Double_t langaufun(Double_t *x, Double_t *par) {
 
   //Fit parameters:
   //par[0]=Width (scale) parameter of Landau density
   //par[1]=Most Probable (MP, location) parameter of Landau density
   //par[2]=Total area (integral -inf to inf, normalization constant)
   //par[3]=Width (sigma) of convoluted Gaussian function
   //
   //In the Landau distribution (represented by the CERNLIB approximation),
   //the maximum is located at x=-0.22278298 with the location parameter=0.
   //This shift is corrected within this function, so that the actual
   //maximum is identical to the MP parameter.
 
      // Numeric constants
      Double_t invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2)
      Double_t mpshift  = -0.22278298;       // Landau maximum location
 
      // Control constants
      Double_t np = 100.0;      // number of convolution steps
      Double_t sc =   5.0;      // convolution extends to +-sc Gaussian sigmas
 
      // Variables
      Double_t xx;
      Double_t mpc;
      Double_t fland;
      Double_t sum = 0.0;
      Double_t xlow,xupp;
      Double_t step;
      Double_t i;
 
 
      // MP shift correction
      mpc = par[1] - mpshift * par[0];
 
      // Range of convolution integral
      xlow = x[0] - sc * par[3];
      xupp = x[0] + sc * par[3];
 
      step = (xupp-xlow) / np;
 
      // Convolution integral of Landau and Gaussian by sum
      for(i=1.0; i<=np/2; i++) {
         xx = xlow + (i-.5) * step;
         fland = TMath::Landau(xx,mpc,par[0]) / par[0];
         sum += fland * TMath::Gaus(x[0],xx,par[3]);
 
         xx = xupp - (i-.5) * step;
         fland = TMath::Landau(xx,mpc,par[0]) / par[0];
         sum += fland * TMath::Gaus(x[0],xx,par[3]);
      }
 
      return (par[2] * step * sum * invsq2pi / par[3]);
}
 
 
 
TF1 *langaufit(TH1D *his, Double_t *fitrange, Double_t *startvalues, Double_t *parlimitslo, Double_t *parlimitshi, Double_t *fitparams, Double_t *fiterrors, Double_t *ChiSqr, Int_t *NDF)
{
   // Once again, here are the Landau * Gaussian parameters:
   //   par[0]=Width (scale) parameter of Landau density
   //   par[1]=Most Probable (MP, location) parameter of Landau density
   //   par[2]=Total area (integral -inf to inf, normalization constant)
   //   par[3]=Width (sigma) of convoluted Gaussian function
   //
   // Variables for langaufit call:
   //   his             histogram to fit
   //   fitrange[2]     lo and hi boundaries of fit range
   //   startvalues[4]  reasonable start values for the fit
   //   parlimitslo[4]  lower parameter limits
   //   parlimitshi[4]  upper parameter limits
   //   fitparams[4]    returns the final fit parameters
   //   fiterrors[4]    returns the final fit errors
   //   ChiSqr          returns the chi square
   //   NDF             returns ndf
 
   Int_t i;
   Char_t FunName[100];
 
   sprintf(FunName,"Fitfcn_%s",his->GetName());
 
   TF1 *ffitold = (TF1*)gROOT->GetListOfFunctions()->FindObject(FunName);
   if (ffitold) delete ffitold;
 
   TF1 *ffit = new TF1(FunName,langaufun,fitrange[0],fitrange[1],4);
   ffit->SetParameters(startvalues);
   ffit->SetParNames("Width","MP","Area","GSigma");
 
   for (i=0; i<4; i++) {
      ffit->SetParLimits(i, parlimitslo[i], parlimitshi[i]);
   }
 
   his->Fit(FunName,"RB0");   // fit within specified range, use ParLimits, do not plot
 
   ffit->GetParameters(fitparams);    // obtain fit parameters
   for (i=0; i<4; i++) {
      fiterrors[i] = ffit->GetParError(i);     // obtain fit parameter errors
   }
   ChiSqr[0] = ffit->GetChisquare();  // obtain chi^2
   NDF[0] = ffit->GetNDF();           // obtain ndf
 
   return (ffit);              // return fit function
 
}

Int_t langaupro(Double_t *params, Double_t &maxx, Double_t &FWHM) {
 
   // Seaches for the location (x value) at the maximum of the
   // Landau-Gaussian convolute and its full width at half-maximum.
   //
   // The search is probably not very efficient, but it's a first try.
 
   Double_t p,x,fy,fxr,fxl;
   Double_t step;
   Double_t l,lold;
   Int_t i = 0;
   Int_t MAXCALLS = 10000;
 
 
   // Search for maximum
 
   p = params[1] - 0.1 * params[0];
   step = 0.05 * params[0];
   lold = -2.0;
   l    = -1.0;
 
 
   while ( (l != lold) && (i < MAXCALLS) ) {
      i++;
 
      lold = l;
      x = p + step;
      l = langaufun(&x,params);
 
      if (l < lold)
         step = -step/10;
 
      p += step;
   }
 
   if (i == MAXCALLS)
      return (-1);
 
   maxx = x;
 
   fy = l/2;
 
 
   // Search for right x location of fy
 
   p = maxx + params[0];
   step = params[0];
   lold = -2.0;
   l    = -1e300;
   i    = 0;
 
 
   while ( (l != lold) && (i < MAXCALLS) ) {
      i++;
 
      lold = l;
      x = p + step;
      l = TMath::Abs(langaufun(&x,params) - fy);
 
      if (l > lold)
         step = -step/10;
 
      p += step;
   }
 
   if (i == MAXCALLS)
      return (-2);
 
   fxr = x;
 
 
   // Search for left x location of fy
 
   p = maxx - 0.5 * params[0];
   step = -params[0];
   lold = -2.0;
   l    = -1e300;
   i    = 0;
 
   while ( (l != lold) && (i < MAXCALLS) ) {
      i++;
 
      lold = l;
      x = p + step;
      l = TMath::Abs(langaufun(&x,params) - fy);
 
      if (l > lold)
         step = -step/10;
 
      p += step;
   }
 
   if (i == MAXCALLS)
      return (-3);
 
 
   fxl = x;
 
   FWHM = fxr - fxl;
   return (0);
}
