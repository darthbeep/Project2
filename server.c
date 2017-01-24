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
      //close(sd2);
      dispatch( connection );

      exit(0);
    }
    else {
      //close( connection );
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
        /* code */
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

    /*
        When the server recieves a message, it needs to know what type of message it is. Therefore, the first two charachtars of a recieved message represent a different code. Here are all of the used codes:

        00: Username
    */
    printf("chatting now. Yay friends\n");
    clock_t t;
    t = clock();
  char buffer[MESSAGE_BUFFER_SIZE];
  char buffer2[MESSAGE_BUFFER_SIZE];

  printf("Not Ended\n");
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
 /* while (read( sd, buffer, sizeof(buffer) ) && read( sd2, buffer2, sizeof(buffer2) ) ) {

    if (buffer[0] == 48 && buffer[1] == 48 && ((buffer2[0] == 48 && buffer2[1] == 48)||(buffer[0] == 0 && buffer[1] == 0)))) {
        //This can never happen. Don't worry about it.
        printf("%s\n", IMPOSSIBLEERROR);
    }
    //Usernames
    else if (buffer[0] == 48 && buffer[1] == 48 && ((buffer2[0] == 48 && buffer2[1] == 48)||(buffer[0] == 0 && buffer[1] == 0))) { //So apparently the char for 0 is 48. That took me way too long to figure out.
        printf("People: %d\n", *people);
        if (1/) {
            printf("Approved\n");

            //clients[*people]->username = buffer + 2;
            accepting = 0;
            write(sd, GOODANSWER, sizeof(GOODANSWER));
            int * transfer = (int *) malloc(sizeof(int));
            *transfer = *people + 1;
            memcpy(people, transfer, sizeof(transfer));
            printf("People after: %d\n", *people);
        }
        else {
            printf("Not Approved\n");
            write(sd, BADANSWER, sizeof(BADANSWER));
        }
        //printf("Got here as well\n");
    }
  }*/

}
void process( char * s ) {
  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}
int search_list_for_usernames(char * potential) {
    printf("%s: %d\n", potential, *people);
    int ret = -1;
    int i = 0;
    for (; i < *people; i++) {
        printf("Client name: %s\n", clients[i]->username);
        if (strcmp(clients[i]->username, potential) == 0) {
            printf("This wont work\n");
            ret = i;
        }
    }
    printf("test\n");
    return ret;
}
