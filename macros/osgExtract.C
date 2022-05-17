Int_t osgExtract(TString fname, TString o_fname){

std::ofstream ofs (o_fname.Data(), std::ofstream::out);

  
TFile *f=new TFile(fname);
TH1F* pe =(TH1F*) f->Get("PhotoElectronDistrHist");
double QuartzSizeX=((TVectorT<double>*) f->Get("QuartzSizeX"))[0][0];
double QuartzSizeY=((TVectorT<double>*) f->Get("QuartzSizeY"))[0][0];
double QuartzSizeZ=((TVectorT<double>*) f->Get("QuartzSizeZ"))[0][0];
double LowerInterfacePlane=((TVectorT<double>*) f->Get("LowerInterfacePlane"))[0][0];
double UpperInterfacePlane=((TVectorT<double>*) f->Get("UpperInterfacePlane"))[0][0];
double LowerConeFrontFaceAngle=((TVectorT<double>*) f->Get("LowerConeFrontFaceAngle"))[0][0];
double LowerConeBackFaceAngle=((TVectorT<double>*) f->Get("LowerConeBackFaceAngle"))[0][0];
double LowerConeSideFaceAngle=((TVectorT<double>*) f->Get("LowerConeSideFaceAngle"))[0][0];
double QuartzInterfaceOpeningZ=((TVectorT<double>*) f->Get("QuartzInterfaceOpeningZ"))[0][0];
double QuartzInterfaceOpeningX=((TVectorT<double>*) f->Get("QuartzInterfaceOpeningX"))[0][0];
double PMTInterfaceOpeningZ=((TVectorT<double>*) f->Get("PMTInterfaceOpeningZ"))[0][0];
double PMTInterfaceOpeningX=((TVectorT<double>*) f->Get("PMTInterfaceOpeningX"))[0][0];
double QuartzToPMTOffsetInZ=((TVectorT<double>*) f->Get("QuartzToPMTOffsetInZ"))[0][0];
double QuartzBevel=((TVectorT<double>*) f->Get("QuartzBevel"))[0][0];
double QuartzRotation=((TVectorT<double>*) f->Get("QuartzRotation"))[0][0];
double DetectorRotation=((TVectorT<double>*) f->Get("DetectorRotation"))[0][0];
double HitRegion=((TVectorT<double>*) f->Get("HitRegion"))[0][0];

TString out="";
out+= Form("%f,", QuartzSizeX);
out+= Form("%f,", QuartzSizeY);
out+= Form("%f,", QuartzSizeZ);
out+= Form("%f,", LowerInterfacePlane);
out+= Form("%f,", UpperInterfacePlane);
out+= Form("%f,", LowerConeFrontFaceAngle);
out+= Form("%f,", LowerConeBackFaceAngle);
out+= Form("%f,", LowerConeSideFaceAngle);
out+= Form("%f,", QuartzInterfaceOpeningZ);
out+= Form("%f,", QuartzInterfaceOpeningX);
out+= Form("%f,", PMTInterfaceOpeningZ);
out+= Form("%f,", PMTInterfaceOpeningX);
out+= Form("%f,", QuartzToPMTOffsetInZ);
out+= Form("%f,", QuartzBevel);
out+= Form("%f,", QuartzRotation);
out+= Form("%f,", DetectorRotation);
out+= Form("%f,", HitRegion);
out+= Form("%f,", pe->GetMean());
out+= Form("%f\n", pow(pe->GetRMS()/pe->GetMean(),2));

f->Close();


ofs << out.Data();
ofs.close();
  
return 0;
}

