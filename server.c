#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "networking.h"

struct person {
    char * username;
    int stage;
};

void process( char * s );
void sub_server( int sd );
int * people;
char accepting = 0;
int touched = 0;
struct person * clients[MAX_PERSON_SIZE]; //= (struct person *) malloc(sizeof(struct person) * MAX_PERSON_SIZE);



int main() {
    //Setting up some shared memory stuff
    int shmid;
    key_t key;
    key = KEY_NUM;
    shmid = shmget(key, MESSAGE_BUFFER_SIZE, IPC_CREAT | 0644);
    people = shmat(shmid, NULL, 0);
    if (people == (int *) -1) {
        printf("%s\n", IMPROBBABLEERROR);
    }
    int * transfer = (int *) malloc(sizeof(int));
    *transfer = 0;
    memcpy(people, transfer, sizeof(transfer));
    printf("Setup went ok\n");
    /*
    int shmid2;
    key_t key2;
    key2 = KEY_NUM + 2;
    shmid = shmget(key2, MESSAGE_BUFFER_SIZE, IPC_CREAT | 0644);
    clients = shmat(shmid2, NULL, 0);
    if (clients == (struct person *) -1) {
        printf("%s\n", IMPROBBABLEERROR);
    }
    int * transfer2 = (struct person *) malloc(sizeof(struct person) * MAX_PERSON_SIZE);
    *transfer2 = (struct person *) malloc(sizeof(struct person) * MAX_PERSON_SIZE);;
    memcpy(clients, transfer2, sizeof(transfer2));
    printf("%s\n", SUCCESS);*/

  int sd, connection;

  sd = server_setup();
  //people = 0;
  printf("Main runs: %d\n", *people);

  while (1) {

    connection = server_connect( sd );

    int f = fork();
    if ( f == 0 ) {

      close(sd);
      sub_server( connection );

      exit(0);
    }
    else {
      close( connection );
    }
  }
  return 0;
}


void sub_server( int sd ) {
    /*
        When the server recieves a message, it needs to know what type of message it is. Therefore, the first two charachtars of a recieved message represent a different code. Here are all of the used codes:

        00: Username
    */

  char buffer[MESSAGE_BUFFER_SIZE];
  printf("Touched: %d\n", touched);
  touched = 77;
  while (read( sd, buffer, sizeof(buffer) )) {
    if (0) {
        //This can never happen. Don't worry about it.
        printf("%s\n", IMPOSSIBLEERROR);
    }
    //Usernames
    else if (buffer[0] == 48 && buffer[1] == 48) { //So apparently the char for 0 is 48. That took me way too long to figure out.
        printf("People: %d\n", *people);
        if (1/*search_list_for_usernames(buffer+2) == -1*/) {
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
        printf("Got here as well\n");
    }
    else {
        printf("This should not be happening. Buffer[0]: %d, Buffer[1]: %d, truth: %d\n", buffer[0], buffer[1], buffer[0] == 0 && buffer[1] == 0);
        printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );
    write( sd, buffer, sizeof(buffer));
    }

  }

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
    for (size_t i = 0; i < *people; i++) {
        printf("Client name: %s\n", clients[i]->username);
        if (strcmp(clients[i]->username, potential) == 0) {
            printf("This wont work\n");
            ret = i;
        }
    }
    printf("test\n");
    return ret;
}
