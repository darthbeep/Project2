#include <sys/shm.>
#include <sys/ipc.h>
#include <sys/types.h>


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
  
  return 0;
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

(<#>,","<username>,"\n")
