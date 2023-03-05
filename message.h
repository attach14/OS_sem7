// message.h
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>

#define PERMS   0666      // права доступа

// структура сообщения, помещаемого в разделяемую память
typedef struct {
  int type;
  int number;
} message_t;

