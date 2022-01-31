#ifndef MOLLEROptPrimaryEvent_h
#define MOLLEROptPrimaryEvent_h

#include "cpp_include.h"
#include "Root_include.h"


class MOLLEROptPrimaryEvent : public TObject
{

private:

  Float_t OriginVertexPositionX;
  Float_t OriginVertexPositionY;
  Float_t OriginVertexPositionZ;

  Int_t   PrimaryEventNumber;

public:
  
  MOLLEROptPrimaryEvent();
  virtual ~MOLLEROptPrimaryEvent();

  void     Initialize();
  void     PrintData();


  //-----------------
  void     StoreOriginVertexPositionX(Float_t vx)   { OriginVertexPositionX = vx; }
  Float_t    GetOriginVertexPositionX() const {return OriginVertexPositionX;}

  void     StoreOriginVertexPositionY(Float_t vy)   { OriginVertexPositionY = vy; }
  Float_t    GetOriginVertexPositionY() const {return OriginVertexPositionY;}

  void     StoreOriginVertexPositionZ(Float_t vz)   { OriginVertexPositionZ = vz; }
  Float_t    GetOriginVertexPositionZ() const {return OriginVertexPositionZ;}
  //-----------------

  //-----------------
  void     StorePrimaryEventNumber(Int_t pen)    { PrimaryEventNumber = pen; }
  Int_t      GetPrimaryEventNumber() const {return PrimaryEventNumber; }
  //-----------------

  ClassDef(MOLLEROptPrimaryEvent,1)

};

#endif 
