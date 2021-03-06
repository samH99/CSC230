#include <stdio.h>
#include <stdlib.h>

// Using a character array to hold a sequence of characters entered
// by the user.
int main( void )
{
  printf( "What's your name: " );

  // Storage for a name, as an array of characters without a null
  // marking the end.
  char name[ 10 ];
  int len = 0;
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF) {
    name[len] = ch;
    len++;
  }
  
  if(len <= 10) {
    printf("Hello ");
    for(int i = 0; i < len; i++) {
    printf("%c", name[i]);
    }
    printf("\n");
  } else {
    printf("That name is too long\n");
  }
   
  // Exit successfully when finished.
  return EXIT_SUCCESS;
}
