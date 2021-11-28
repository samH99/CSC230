#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>

// Representation for a 2D point with integer coordinates.
typedef struct {
  // Coordinates of the point.
  int x, y;
} Point;

// Print out an error message and exit.
static void fail( char const *message ) {
  fprintf( stderr, "%s\n", message );
  exit( 1 );
}

// List of all points from the input
Point *ptList;

// Number of points in the input.
int ptCount = 0;

// Read the list of all points
void readPoints() {
  // Use a resizable array, increasing capacity as we read
  int capacity = 10;
  ptList = (Point *) malloc( capacity * sizeof( Point ) );

  // Read points until we can't read any more.
  int x, y;
  while ( scanf( "%d%d", &x, &y ) == 2 ) {
    // Grow the point list if needed.
    if ( ptCount >= capacity ) {
      capacity *= 2;
      ptList = (Point *) realloc( ptList, capacity * sizeof( Point ) );
    }
    
    // Add this new point to the end of the list.
    ptList[ ptCount ].x = x;
    ptList[ ptCount ].y = y;
    ptCount += 1;
  }
}

// Return the squared distance between point i and point j in the ptList.
int distSquared( int i, int j ) {
  int dx = ptList[ i ].x - ptList[ j ].x;
  int dy = ptList[ i ].y - ptList[ j ].y;
  return (dx * dx + dy * dy);
}

int main( int argc, char *argv[] ) {
  // Read in the number of cores
  int numOfCores = atoi(argv[1]);

  // Read in all the points
  readPoints();
  //int triangleCount = 0;

  // printf("%d = the point count\n", ptCount);

  // for (int a = 0; a < ptCount; a++){
  //   for(int b = 0; b < ptCount; b++){
  //     for(int c = 0; c < ptCount; c++){
  //       double ds1 = sqrt(distSquared(a,b));
  //       double ds2 = sqrt(distSquared(a,c));
  //       double ds3 = sqrt(distSquared(b,c));

  //       if(ds1 != 0 && ds2 != 0 && ds3 != 0){
  //         if(((ds1*0.9) <=ds2) && ((ds1*1.1) >=ds2)){
  //           if(((ds1*0.9) <=ds3) && ((ds1*1.1) >=ds3)){
  //             if(((ds2*0.9) <=ds3) && ((ds2*1.1) >=ds3)){
  //               if(((ds2*0.9) <=ds1) && ((ds2*1.1) >=ds1)){
  //                 if(((ds3*0.9) <=ds1) && ((ds3*1.1) >=ds1)){
  //                   if(((ds3*0.9) <=ds2) && ((ds3*1.1) >=ds2)){
  //                     triangleCount++;
  //                   }
  //                 }
  //               }
  //             }
  //           }
  //         }
  //       }
  //     }
  //   }
  // }

  // printf("%d = the triangle count\n", (triangleCount)/6);




  // Make a pipe for getting counts back from our workers.
  int pfd[ 2 ];
  if ( pipe( pfd ) != 0 ){
    fail( "Can't create pipe" );
  }

  // Make all the workers, and let them solve their parts of the problem.
  // ...
  for(int c = 0; c < numOfCores; c++){
    pid_t pid = fork(); 
    if ( pid < 0 ){
      fail( "Not able to fork a child");
    } else{
      printf("child forked\n");
    }
  
    if ( pid == 0 ) {
      printf( "\nchild start\n" );
      // Close the reading end of the pipe.  We don't need it.
      close( pfd[ 0 ] );
      
      // Read a string message from the user.
  
      // Send the message over the pipe.
      int n = 1;
  
      // Push message into pipe, until it's all written.
      lockf(pfd[ 1 ], F_LOCK, 0 );
      write(pfd[ 1 ], &n, sizeof(n));
      lockf(pfd[ 1 ], F_ULOCK, 0 );
  
      close( pfd[ 1 ] );
      printf( "\nchild end\n" );
      return 0;
    }
  }

  // Get a count back from each worker
  // ... 

  // Wait for all the workers to finish.
  // ...

  // Report the total number of triangles we found.
  // ..
  

  
  printf( "\nparent start\n" );
  
  // I'm the parent, I don't need the writing end of the pipe.
  close( pfd[ 1 ] );
  int sum = 0;
  int n = 0;
  while (read(pfd[0], &n, sizeof(n)) > 0) {
    sum+=n;
  }
  wait( NULL );
  printf( "%d \n" , sum);

  // Wait for our child to exit.
  
  printf( "\nparent end\n" );
  
  return 0;
}
