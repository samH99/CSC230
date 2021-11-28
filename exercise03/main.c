#include <stdio.h>
#include <stdlib.h>
#include "reader.h"
#include "writer.h"

int main()
{
  int ch;

  // Keep reading characters until get tells us EOF.
  while ( ( ch = get() ) != EOF )
    put( ch );

  return EXIT_SUCCESS;
}
