#include "backward.h"
#include "tm.h"


HaltingConfiguration* hc_new()
{
  HaltingConfiguration* hc = malloc(sizeof(HaltingConfiguration));
  if(!hc)
    return NULL;
  
  return hc;
}

void run_br(TuringMachine *tm)
{
  if(!tm)
    return;

  HaltingConfiguration* root;
  
  
  return;
}