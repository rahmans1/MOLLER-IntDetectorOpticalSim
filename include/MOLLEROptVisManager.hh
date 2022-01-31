#ifndef MOLLEROptVisManager_h
#define MOLLEROptVisManager_h 1

#ifdef G4VIS_USE

#include "G4VisManager.hh"

class MOLLEROptVisManager: public G4VisManager {

public:

  MOLLEROptVisManager ();

private:

  void RegisterGraphicsSystems ();

};

#endif

#endif
