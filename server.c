#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include "files.c"
#include "networking.h"

struct person {
    char * username;
    int stage;
};

void process( char * s );
void dispatch( int []);
void converse( int sd, int sd2 );
int * people;
//int * end;
char accepting = 0;
int touched = 0;
struct person * clients[MAX_PERSON_SIZE]; //= (struct person *) malloc(sizeof(struct person) * MAX_PERSON_SIZE);



int main() {
    //Setting up some shared memory stuff
    int shmid;
    key_t key;
    key = KEY_NUM;
    shmid = shmget(key, MESSAGE_BUFFER_SIZE, IPC_CREAT | 0644);
    end = shmat(shmid, NULL, 0);
    if (end == (int *) -1) {
        printf("%s\n", IMPROBBABLEERROR);
    }
    int * transfer = (int *) malloc(sizeof(int));
    *transfer = 0;
    printf("Setup went ok\n");

  int sd;
  int connection[NUMBER_PEOPLE];

  sd = server_setup();

  while (1) {
    int i = 0;
    for (; i < NUMBER_PEOPLE; i++) {
        connection[i] = server_connect(sd);
    }
    int f = fork();
    if ( f == 0 ) {
      close(sd);
      dispatch( connection );

      exit(0);
    }
    else {
      int i = 0;
      for (; i < NUMBER_PEOPLE; i++) {
          close(connection[i]);
      }
    }

  }
  return 0;
}

void dispatch(int connection[MAX_PERSON_SIZE]) {
    int orig = getpid();
    //This is where you put in a clever pairing algorithm. Right now the argorithm for 4 people is hardcoded in
    int i = 0;
    for (; i < NUMBER_PEOPLE; i++) {
        write(connection[i], SUCCESS, sizeof(SUCCESS));
    }
    if (getpid() == orig) {
        int f = fork();
    if (f == 0) {
        converse(connection[0], connection[1]);
    }
    else {
        converse(connection[2], connection[3]);
    }
    }

    if (getpid() == orig) {
        int g = fork();
        if (g == 0) {
            converse(connection[0], connection[2]);
        }
        else {
            converse(connection[1], connection[3]);
        }
    }
    if (getpid() == orig) {
        int h = fork();
        if (h == 0) {
            converse(connection[0], connection[3]);
        }
        else {
            converse(connection[1], connection[2]);
        }
    }
    if (getpid() == orig) {
      int i = 0;
      for (; i < NUMBER_PEOPLE; i++) {
	  write(connection[i], ENDING, sizeof(ENDING));
        }
    }
    while (1) {
    }
}

void converse( int sd, int sd2 ) {
  int r = rand() % 50;
  char stringer[100];

  sprintf(stringer, "Question: %s\n", getQuestion(r));

  write(sd, stringer, strlen(stringer));
  write(sd2, stringer, strlen(stringer));

  write(sd, START, sizeof(START));
  write(sd2, START, sizeof(START));

  char buffer[MESSAGE_BUFFER_SIZE];
  char buffer2[MESSAGE_BUFFER_SIZE];
  int f = fork();
  if (f == 0) {
      while (read( sd, buffer, sizeof(buffer) )) {


          if (strcmp(buffer, KILL_ME) == 0) {
              return;
             printf("Hi\n");
         }
          write(sd2, buffer, sizeof(buffer));
      }
  }
  else {
      while (read( sd2, buffer2, sizeof(buffer2) )) {


          if (strcmp(buffer2, KILL_ME) == 0) {
              return;
          }
          write(sd, buffer2, sizeof(buffer2));
      }

  }
exit(0);
}
