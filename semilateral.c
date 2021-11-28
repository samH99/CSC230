#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>

// Print out an error message and exit.
static void fail( char const *message ) {
  fprintf( stderr, "%s\n", message );
  exit( 1 );
}

// Representation for a 2D point with integer coordinates.
typedef struct {
  // Coordinates of the point.
  int x, y;
} Point;

// Maximum number of points we can store on ptList.  For this program,
// the list is allocated at a fixed size at the start of execution.
// This letsd so workers start working on Points as soon as we read
// them in, without having to worry about the list moving in memory
// (if, say, it had to get resized).
#define POINT_LIMIT 10000

// List of all points from the input
Point *ptList;

// Number of points in the input.
int ptCount = 0;

// The number of workers
int workers;

// Total number of approximately equilateral triangles we've found
int total = 0;

// Read the lsit of points.
void readPoints() {
  // Read points until we can't read any more.
  int x, y;
  while ( scanf( "%d%d", &x, &y ) == 2 ) {
    if ( ptCount >= POINT_LIMIT )
      fail( "Too many points." );

    // Append the point we just read to the list.
    ptList[ ptCount ].x = x;
    ptList[ ptCount ].y = y;
    ptCount++;
  }
}

// Return the squared distance between point i and point j in the ptList.
int distSquared( int i, int j ) {
  int dx = ptList[ i ].x - ptList[ j ].x;
  int dy = ptList[ i ].y - ptList[ j ].y;
  return (dx * dx + dy * dy);
}

int getWork( int index ){
  if (feof(stdin))
    return 2;
  if (index <= (ptCount - 1))
    return 0;
  return 1;
}

// Anonymous semaphore for mutual exclusion of the total variable
sem_t gotTotal;

/** Start routine for each worker. */
void *workerRoutine( void *arg ) {

  int triangleCount = 0;

  for (int a = *(int *)arg; ; a+=workers){
    if(a > ptCount){
      if(getWork(a) == 2)
        break;
      while(getWork(a) == 1){
        printf( "Triangles: " );
      }
    }
    for(int b = a - 1; b > 0; b--){
      for(int c = b - 1; c > 0; c--){
        double ds1 = sqrt(distSquared(a,b));
        double ds2 = sqrt(distSquared(a,c));
        double ds3 = sqrt(distSquared(b,c));

        if(ds1 != 0 && ds2 != 0 && ds3 != 0){
          if(((ds1*0.9) <=ds2) && ((ds1*1.1) >=ds2)){
            if(((ds1*0.9) <=ds3) && ((ds1*1.1) >=ds3)){
              if(((ds2*0.9) <=ds3) && ((ds2*1.1) >=ds3)){
                if(((ds2*0.9) <=ds1) && ((ds2*1.1) >=ds1)){
                  if(((ds3*0.9) <=ds1) && ((ds3*1.1) >=ds1)){
                    if(((ds3*0.9) <=ds2) && ((ds3*1.1) >=ds2)){
                      triangleCount++;
                    }
                  }
                }
              }
            }
          }
        }
        
      }
    }
  }

  sem_wait( &gotTotal );
  
  total += triangleCount;

  sem_post( &gotTotal );
  return NULL;
}
  
int main( int argc, char *argv[] ) {
  
  printf( "0");
  // Create a semephaore for mutual exclusion of total
  sem_init( &gotTotal, 0, 1 );
  
  printf( "1");

  int workers = 4;
  if ( argc != 2 ||
       sscanf( argv[ 1 ], "%d", &workers ) != 1 ||
       workers < 1 )
    fail( "usage: semilateral <workers>" );

  // Allocate our statically-sized list for points.
  ptList = (Point *) malloc( POINT_LIMIT * sizeof( Point ) );

  printf( "2");

  // Make each of the workers.
  pthread_t worker[ workers ];
  for ( int i = 0; i < workers; i++ ){
    if ( pthread_create( &worker[i], NULL, workerRoutine, &i ) != 0 ){
      fail( "Can't create a child thread" );
    }
  }

  printf( "3");

  // Then, start getting work for them to do.
  readPoints();

  // Wait until all the workers finish.
  for ( int i = 0; i < workers; i++ ){
    pthread_join( worker[i], NULL );
  }

  printf( "4");

  // Report the total and release the semaphores.
  printf( "Triangles: %d\n", total );
  sem_destroy( &gotTotal );
  
  printf( "5");

  return EXIT_SUCCESS;
}
