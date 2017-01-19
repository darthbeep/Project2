#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

int serverOpenMemory(char* fileName){
  key_t key;
  int shmid;
  char *data;
  int mode;
  
  if ((key = ftok(fileName, 'R')) == -1) 
    {
      perror("ftok");
      exit(1);
    }

  if ((shmid = shmget(key, 2048, 0644 | IPC_CREAT)) == -1) {
    perror("shmget");
    exit(1);
  }
  data = shmat(shmid, (void *)0, 0);
  if (data == (char *)(-1)) {
    perror("shmat");
    exit(1);
  }
  
  return shmid;
}

int clientAccess(int shmid){
  char * data = shmat(shmid, (void *)0, 0);
  if (data == (char *)(-1)) {
    perror("shmat");
    exit(1);
  }
}
    

int serverStartUp(){
  serverOpenMemory("usernames.txt");
}
  

int serverDetach(char* data){
  if (shmdt(data) == -1) {
    perror("shmdt");
    exit(1);
  }
}

int usernameCheck(char * username){
  FILE *fp;
  fp=fopen("username.txt", "r");
  fseek(fp, 0, SEEK_END);
  long fsize = ftell(fp);
  rewind(fp);
  char *str = malloc(fsize + 1);
  fread(str, fsize, 1, fp);
  fclose(fp);
}

  
/* (<#>,","<username>,"\n") */
int usernameAdd(char * username){
  FILE *fp;
  fp=fopen("username.txt", "a+");
  char * str;
  strcpy(str, "100");
  strcat(str, ",");
  strcat(str, username);
  strcat(str, "\n");

  /* char * line = "1" + "," + username + "\n"; */

  
  fprintf(fp,"%s",str);
  fclose(fp);
}

int main(){
  char* x= "brandon";
  usernameAdd(x);}
