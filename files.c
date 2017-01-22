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


//returns number of lines in files
int numberLines(char * file){
  FILE *fp;
  fp=fopen(file, "r");
  fseek(fp, 0, SEEK_END);
  long fsize = ftell(fp);
  rewind(fp);
  char *str = malloc(fsize + 1);
  fread(str, fsize, 1, fp);
   printf("%s",str);
  //str holds the file
  int i = 0;
   
  printf("%s",str);
  while (strsep(&str,"\n") != NULL){
    printf("%s,%d",str,i);
    i++;} 
  /* str = strsep(&str, "\n"); */
  /* printf("%s",str); */
  
  free(str);
  fclose(fp);
  return i;
}


	 
//returns username number from username.txt if it exists. 0 otherwise.
int usernameNumber(char * username){
  FILE *fp;
  fp=fopen("username.txt", "r");
  fseek(fp, 0, SEEK_END);
  long fsize = ftell(fp);
  rewind(fp);
  char *str = malloc(fsize + 1);
  fread(str, fsize, 1, fp);
   printf("%s",str);
  //str holds the file
 
  
 str = strstr(str, username);
  if (str == NULL) {
    char *temp = str;
    str = NULL;
    return 0;
    
  }

  char *temp = str;

  *str = '\0';
  str = str + strlen(username );

  int i = 0;
   
  printf("%s",str);
  while (strsep(&str,"\n") != NULL){
    /* printf("%s,%d",str,i); */
    i++;} 
  
  /* str = strsep(&str, "\n"); */
  /* printf("%s",str); */
  /* free(str); */
  fclose(fp);
  return numberLines("username.txt") - i;

  

  

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
  free(str);
}

int main(){
  char* f = "username.txt";
  char* x= "matt";
  printf("%d",usernameNumber(x));
 
}
