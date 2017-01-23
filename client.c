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
  int f = fork();
  if (f == 0) {
      while (1) {
          read(sd, buffer, sizeof(buffer));
          printf("%s\n", buffer);
      }
  }
  //Gather username
  //while (usernameenterred == 0) {
       printf("Enter your username\n");
       char * submitusername = (char *) malloc(sizeof(char));
       fgets(buffer, sizeof(buffer), stdin);
       char *fix = strchr(buffer, '\n');
       *fix = 0;
       strcpy(username, buffer);
       //strcat(submitusername, "00"); //00 communicates that it is a test for usernames
       //strcat(submitusername, buffer);
       //write(sd, submitusername, sizeof(buffer));
       /*read(sd, buffer, sizeof(buffer));
       printf("Buffer: %s\n", buffer);
       if (strcmp(buffer, GOODANSWER) == 0) {
           usernameenterred = 1;
           printf("You may proceed\n");
       }*/
  //}
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
    //printf( "received: %s\n", buffer );
  }

  return 0;
}
