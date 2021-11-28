#include <stdio.h>
#include "writer.h"

/** Writes characters out to standard output, but refuses to wrte
    carriage returns or line feeds, so all the output will be on one
    line. */
void put( int ch )
{
  if ( ch != '\r' && ch != '\n' )
    putchar( ch );
}
