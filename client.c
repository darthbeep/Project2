#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

//char * username;
int main( int argc, char *argv[] ) {
    //username = (char *) malloc(sizeof(char));
    char username[MESSAGE_BUFFER_SIZE];
  char *host;
  if (argc != 2 ) {
    printf("host not specified, conneting to 127.0.0.1\n");
    host = "127.0.0.1";
  }
  else
    host = argv[1];

  int sd;

  sd = client_connect( host );

  char buffer[MESSAGE_BUFFER_SIZE];
  char usernameenterred = 0;

  //Make sure to read everything

  //Gather username

  read(sd, buffer, sizeof(buffer));
       printf("Enter your username\n");
       char * submitusername = (char *) malloc(sizeof(char));
       fgets(buffer, sizeof(buffer), stdin);
       char *fix = strchr(buffer, '\n');
       *fix = 0;
       strcpy(username, buffer);

       int f = fork();
  if (f == 0) {
      while (1) {
          read(sd, buffer, sizeof(buffer));
          //if (strcmp(buffer, START) == 0) {
              int g = fork();
              if (g == 0) {
                  sleep(20);
                  write(sd, KILL_ME, sizeof(KILL_ME));
                  exit(1);
		  }
	      //}
          printf("%s\n", buffer);
      }
  }

printf("Username: %s\n", username);
  while (1) {
    //printf("enter message: ");
    fgets( buffer, sizeof(buffer), stdin );
    char *p = strchr(buffer, '\n');
    *p = 0;
    char send[MESSAGE_BUFFER_SIZE];
    strcpy(send, username);
    strcat(send, ": ");
    strcat(send, buffer);

    write( sd, send, sizeof(send) );
    //read( sd, buffer, sizeof(buffer) );
    printf( "received: %s\n", buffer );
   
  }

  return 0;
}
