#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>

/** Port number used by my server */
#define PORT_NUMBER "26132"

// Capacity of the list
int size = 5;

//mutex monitor
pthread_mutex_t mon;

//mutex conditional
pthread_cond_t locked;

// List of all names
static char **names;

// Number of names currently on Items.
int len = 0;

// adds the name given to the name list
void add(char* name) {
  if(len == size){
    names = realloc(names, size*2 * sizeof(*names));
    size = size*2;

    for(int i=len; i<size; i++ ) {
      names[i] = malloc(24+1 * sizeof(char));
    }
  }
  strcpy(names[ len ], name);
  len++;
}

// deletes the name given from the name list
void delete(char* name) {
  for ( int i = 0; i < len; i++ ){
    if(strcmp(names[ i ], name) == 0){
      strcpy(names[ i ], "");
    }
  }
}

// retruns 1 if a name is locked, 0 otherwise
int isLocked(char* name) {
  for ( int i = 0; i < len; i++ ){
    if(strcmp(names[ i ], name) == 0){
      return 1;
    }
  }
  return 0;
}

// Print out an error message and exit.
static void fail( char const *message ) {
  fprintf( stderr, "%s\n", message );
  exit( 1 );
}

/** handle a client connection, close it when we're done. */
void *handleClient( void * arg ) {
  // Here's a nice trick, wrap a C standard IO FILE around the
  // socket, so we can communicate the same way we would read/write
  // a file.

  int *param = (int *)arg;
  int sock = param[0];
  FILE *fp = fdopen( sock, "a+" );
  
  // Prompt the user for a command.
  fprintf( fp, "cmd> " );

  // command read
  char cmd[ 11 ];
  // name read
  char name[ 24 + 1 ];

  int match;
  while ( ( match = fscanf( fp, "%s", cmd ) ) == 1 &&
          strcmp( cmd, "quit" ) != 0 ) {

    //fail if the command is too long
    if(strlen(cmd) > 6){
      fail("Input too long");
    }

    //lock before we access the list
    pthread_mutex_lock( &mon );

    // if the command is lock put the name in the list
    if(strcmp( cmd, "lock" ) == 0){
      fscanf( fp, "%s", name );

      //fail if the name is too long
      if(strlen(name) > 24){
        fail("Input too long");
      }
      //wait if the name is already locked
      while(isLocked(name) == 1){
        pthread_cond_wait( &locked, &mon );
      }
      add(name);
    }

    // if the command is unlock then delete the name from the list
    if(strcmp( cmd, "unlock" ) == 0){
      fscanf( fp, "%s", name );

      //fail if the name is too long
      if(strlen(name) > 24){
        fail("Input too long");
      }
      delete(name);

      //signal waiting threads
      pthread_cond_signal(&locked);
    }

    // if the command is list then print out the names list
    if(strcmp( cmd, "list" ) == 0){
      for ( int i = 0; i < len; i++ ){
        if(strlen(names[ i ]) != 0){
          fprintf( fp, "%s\n", names[ i ]);
        }
      }
    }

    // unlock the monitor
    pthread_mutex_unlock( &mon );

    // Prompt the user for the next command.
    fprintf( fp, "cmd> " );
    fflush( fp );
  }

  // Close the connection with this client.
  fclose( fp );

  return NULL;
}

int main() {
  //initialize the monitor and the conditional variable
  pthread_mutex_init(&mon, NULL);
  pthread_cond_init(&locked, NULL);

  //allocate memory for the names list
  names = malloc( size * sizeof(*names) );

  //allocate memory for the names
  for(int i=0; i<size; i++ ) {
    names[i] = malloc(24+1 * sizeof(char));
  }

  // Prepare a description of server address criteria.
  struct addrinfo addrCriteria;
  memset(&addrCriteria, 0, sizeof(addrCriteria));
  addrCriteria.ai_family = AF_INET;
  addrCriteria.ai_flags = AI_PASSIVE;
  addrCriteria.ai_socktype = SOCK_STREAM;
  addrCriteria.ai_protocol = IPPROTO_TCP;

  // Lookup a list of matching addresses
  struct addrinfo *servAddr;
  if ( getaddrinfo( NULL, PORT_NUMBER, &addrCriteria, &servAddr) )
    fail( "Can't get address info" );

  // Try to just use the first one.
  if ( servAddr == NULL )
    fail( "Can't get address" );

  // Create a TCP socket
  int servSock = socket( servAddr->ai_family, servAddr->ai_socktype,
                         servAddr->ai_protocol);
  if ( servSock < 0 )
    fail( "Can't create socket" );

  // Bind to the local address
  if ( bind(servSock, servAddr->ai_addr, servAddr->ai_addrlen) != 0 )
    fail( "Can't bind socket" );
  
  // Tell the socket to listen for incoming connections.
  if ( listen( servSock, 5 ) != 0 )
    fail( "Can't listen on socket" );

  // Free address list allocated by getaddrinfo()
  freeaddrinfo(servAddr);

  // Fields for accepting a client connection.
  struct sockaddr_storage clntAddr; // Client address
  socklen_t clntAddrLen = sizeof(clntAddr);

  //sock. i like socks i wear them all the time
  int sock;

  //the pthread
  pthread_t thred;
  while ( (sock = accept( servSock, (struct sockaddr *) &clntAddr, &clntAddrLen)) != -1) {
    //create and detach the thread
    pthread_create( &thred, NULL, &handleClient, (void *)&sock);
    pthread_detach(thred);
  }

  // Stop accepting client connections (never reached).
  close( servSock );
  
  return 0;
}
