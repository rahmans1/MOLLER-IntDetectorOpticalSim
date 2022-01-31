#include "MOLLEROptPrimaryEvent.hh"

MOLLEROptPrimaryEvent::MOLLEROptPrimaryEvent()   
{
  Initialize();
} 

MOLLEROptPrimaryEvent::~MOLLEROptPrimaryEvent()
{

}

void MOLLEROptPrimaryEvent::Initialize()
{
  OriginVertexPositionX          = 0.0;
  OriginVertexPositionY          = 0.0;
  OriginVertexPositionZ          = 0.0;
}

void MOLLEROptPrimaryEvent::PrintData()
{
  printf("OriginVertexPositionX     = %f\n",OriginVertexPositionX     );
  printf("OriginVertexPositionY     = %f\n",OriginVertexPositionY     );
  printf("OriginVertexPositionZ     = %f\n",OriginVertexPositionZ     );
}
