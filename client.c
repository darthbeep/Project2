#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

int main( int argc, char *argv[] ) {

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
  //Gather username
  while (usernameenterred == 0) {
       printf("Enter your username\n");
       char * submitusername = (char *) malloc(sizeof(char));
       fgets(buffer, sizeof(buffer), stdin);
       char *fix = strchr(buffer, '\n');
       *fix = 0;
       strcat(submitusername, "00"); //00 communicates that it is a test for usernames
       strcat(submitusername, buffer);
       write(sd, submitusername, sizeof(buffer));
       read(sd, buffer, sizeof(buffer));
       printf("Buffer: %s\n", buffer);
       if (strcmp(buffer, GOODANSWER) == 0) {
           usernameenterred = 1;
           printf("You may proceed\n");
       }
  }

  while (1) {
    printf("enter message: ");
    fgets( buffer, sizeof(buffer), stdin );
    char *p = strchr(buffer, '\n');
    *p = 0;

    write( sd, buffer, sizeof(buffer) );
    read( sd, buffer, sizeof(buffer) );
    printf( "received: %s\n", buffer );
  }

  return 0;
}
