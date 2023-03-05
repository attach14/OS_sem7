// shared-memory-client.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include "message.h"

const char* shar_object = "posix-shar-object";

void sys_err (char *msg) {
  puts (msg);
  exit (1);
}

int main (int argc, char* argv[]) {
  srand(time(NULL));
  int shmid;            // дескриптор разделяемой памяти
   message_t *msg_p;      // адрес сообщения в разделяемой памяти
   if ( (shmid = shm_open(shar_object, O_CREAT|O_RDWR, 0666)) == -1 ) {
    perror("shm_open");
    sys_err ("client: object is already open");
  } else {
    printf("Object is open: name = %s, id = 0x%x\n", shar_object, shmid);
  }
   //получить доступ к памяти
   msg_p = mmap(0, sizeof (message_t), PROT_WRITE|PROT_READ, MAP_SHARED, shmid, 0);
   if(argc != 2){ //проверяем, что было передано ровно одно число - количество генераций
   	printf("Wrong number of input arguments: %d instead of 1\n", argc - 1);
   	msg_p->type = 2;
   	return 0;
   }
   int x = atoi(argv[1]);
   if(x<=0 || x >=1000) { //проверяем, что число лежит в нужном диапазоне
   	printf("Number of generations must be more than 0 and less than 1000\n");
   	msg_p->type = 2;
   	return 0;
   }
  
  // Организация передачи сообщений
  while(x){
    if(msg_p->type == 3) {
    msg_p->number = random() % 1000;
    printf("random number = %d\n", msg_p->number);
    msg_p->type = 1;
    x--;
    }
 }
    while(1) {
    if (msg_p->type == 3) { // сервер вывел последнее число
    msg_p->type = 2; // сообщаем ему, что надо завершать программу
    break;
}
}
  close(shmid);
  return 0;
}

