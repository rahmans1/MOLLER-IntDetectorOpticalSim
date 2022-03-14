#ifndef MOLLEROptRunAction_h
#define MOLLEROptRunAction_h 1

#include "cpp_include.h"
#include "Root_include.h"
#include "Geant4_include.hh" 

#include "MOLLEROptAnalysis.hh"
#include "MOLLEROptRunActionMessenger.hh"

class MOLLEROptRunActionMessenger;
class MOLLEROptAnalysis;

class MOLLEROptRunAction : public G4UserRunAction
{
public:
  MOLLEROptRunAction(MOLLEROptAnalysis* AN, MOLLEROptTrackingReadout* TrRO);
  ~MOLLEROptRunAction();

public:
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);
  G4int  getRunID();
  void SetMyRunID(G4int id) {MyRunID = id;};
  void SetROOTFileFlag(G4int flag) {ROOTFileFlag = flag;};
  void SetMyRunFileName(G4String str){runFileName = str;};
  void SetMyRunDirName(G4String str){runDirName = str;};
  G4String GetRunFileName(){return runFileName;};
  
private:

  G4int runID;
  G4String runFileName;
  G4String runDirName;
  G4int MyRunID;
  G4int ROOTFileFlag;
  MOLLEROptAnalysis* analysis;
  MOLLEROptTrackingReadout* TrackingReadout;
  MOLLEROptRunActionMessenger *RunActionMessenger;

};

#endif
