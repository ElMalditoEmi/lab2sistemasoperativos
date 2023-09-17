#ifndef SEM_H
#define SEM_H

#include "spinlock.h"
#define maxsem 10
struct semaphore
{
    int count; // Valor binario del semaforo
               // 0 == Bloqueado 1 == Libre
    struct spinlock lock;
};

#endif