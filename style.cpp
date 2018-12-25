

#include "style.h"

void function();  // empty

char* run(int val, char* valTwo, bool valThree)
{
  val      = 1;
  valTwo   = &valOne;
  valThree = false;

  function();

  return 0;
}

void function()
{
  NewType typeNew;
  if (typeNew.Size() == 0) {
    // Do something
  } else {  // Always wrap else with braces
            // Do something else
  }
}