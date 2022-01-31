#ifndef MOLLEROptMainEvent_h
#define MOLLEROptMainEvent_h

#include "cpp_include.h"
#include "Root_include.h"

// user includes
#include "MOLLEROptPrimaryEvent.hh"
#include "MOLLEROptDetectorEvent.hh"    

class MOLLEROptPrimaryEvent;   
class MOLLEROptDetectorEvent;    

class MOLLEROptMainEvent : public TObject
{

private:

public:
    
  MOLLEROptPrimaryEvent          MOLLERPrimEvent;
  MOLLEROptDetectorEvent         MOLLERDetectorEvent;

public:

  MOLLEROptMainEvent();

  virtual ~MOLLEROptMainEvent();
  
  ClassDef(MOLLEROptMainEvent,1)

};

#endif 
